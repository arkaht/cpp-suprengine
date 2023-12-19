#include "player-spaceship-controller.h"

#include <suprengine/easing.h>

#include <components/player-hud.h>

using namespace puzzle;

PlayerSpaceshipController::PlayerSpaceshipController()
{
	create_component_pro<PlayerHUD>( this );

	//  setup camera settings
	CameraProjectionSettings projection_settings {};
	projection_settings.znear = 1.0f;
	projection_settings.zfar = 10000.0f;

	//  initialize camera
	auto camera_owner = new Entity();
	camera = camera_owner->create_component<Camera>( projection_settings );
	camera->activate();

	//  test: second camera
	camera_owner = new Entity();
	camera_owner->transform->scale = Vec3( 10.0f );
}

PlayerSpaceshipController::~PlayerSpaceshipController()
{
	camera->get_owner()->kill();
}

void PlayerSpaceshipController::update_this( float dt )
{
	if ( !_possessed_ship ) return;

	_update_shoot( dt );
	_update_camera( dt );
}

void PlayerSpaceshipController::on_possess()
{
}

void PlayerSpaceshipController::on_unpossess()
{
}

void PlayerSpaceshipController::update_inputs( float dt )
{
	auto input_manager = _engine->get_inputs();

	//  movement inputs
	_inputs.throttle_delta = input_manager->get_keys_as_axis( 
		SDL_SCANCODE_S, SDL_SCANCODE_W, 1.0f );

	//  rotation inputs
	Vec2 mouse_delta = -input_manager->mouse_delta;
	float yaw_delta = input_manager->get_keys_as_axis( 
		SDL_SCANCODE_A, SDL_SCANCODE_D, 1.0f );
	float roll_delta = mouse_delta.x;
	float pitch_delta = mouse_delta.y;

	//  handle aim velocity
	{
		//  add inputs
		_aim_velocity.x = math::clamp( 
			_aim_velocity.x + roll_delta * AIM_SENSITIVITY.x, 
			-MAX_AIM_VELOCITY, 
			MAX_AIM_VELOCITY 
		);
		_aim_velocity.y = math::clamp( 
			_aim_velocity.y + pitch_delta * AIM_SENSITIVITY.y, 
			-MAX_AIM_VELOCITY, 
			MAX_AIM_VELOCITY 
		);
		_aim_velocity.z = math::clamp(
			_aim_velocity.z + yaw_delta * AIM_SENSITIVITY.z,
			-MAX_AIM_VELOCITY,
			MAX_AIM_VELOCITY
		);

		//  drag to zero
		_aim_velocity = Vec3::lerp( 
			_aim_velocity, 
			Vec2::zero, 
			dt * AIM_VELOCITY_DECREASE
		);
	}

	//  input rotation
	Quaternion rotation = _possessed_ship->transform->rotation;
	rotation = rotation + Quaternion( 
		_possessed_ship->transform->get_forward(), 
		_aim_velocity.x * dt 
	);
	rotation = rotation + Quaternion( 
		_possessed_ship->transform->get_right(), 
		_aim_velocity.y * dt 
	);
	rotation = rotation + Quaternion(
		_possessed_ship->transform->get_up(),
		_aim_velocity.z * dt
	);
	_inputs.desired_rotation = rotation;
}

void PlayerSpaceshipController::_update_shoot( float dt )
{
	auto input_manager = _engine->get_inputs();

	if ( _possessed_ship->get_shoot_time() > 0.0f ) return;
	if ( !input_manager->is_mouse_button_down( MouseButton::Left ) ) return;

	//  shoot
	_possessed_ship->shoot();
}

void PlayerSpaceshipController::_update_camera( float dt )
{
	auto input_manager = _engine->get_inputs();

	//  get parameters
	const float throttle = _possessed_ship->get_throttle();
	const float throttle_ratio = easing::in_out_cubic( 
		throttle / 1.0f );
	const float smooth_move_speed = math::lerp( 
		CAMERA_MOVE_SPEED.x, CAMERA_MOVE_SPEED.y, throttle_ratio );
	const float up_distance = math::lerp( 
		CAMERA_UP_RANGE.x, CAMERA_UP_RANGE.y, throttle_ratio );

	//  get axes
	const Vec3 up = _possessed_ship->transform->get_up();
	const Vec3 forward = _possessed_ship->transform->get_forward();
	
	//  find targets location & rotation
	Vec3 target_location = _possessed_ship->transform->location
	  + up * up_distance;
	Quaternion target_rotation = _possessed_ship->transform->rotation;
	if ( input_manager->is_mouse_button_down( MouseButton::Right ) )
	{
		float distance = math::lerp( 
			CAMERA_LOOK_BACKWARD_DISTANCE.x, 
			CAMERA_LOOK_BACKWARD_DISTANCE.y, 
			throttle  //  looks better on linearly progressive distance
		);
		target_location += forward * distance;
		target_rotation = Quaternion::concatenate( 
			target_rotation, 
			Quaternion( up, math::PI ) 
		);
	}
	else
	{
		float distance = math::lerp( 
			CAMERA_BACKWARD.x, 
			CAMERA_BACKWARD.y, 
			throttle_ratio 
		);
		target_location += forward * -distance;
	}

	//  apply location & rotation
	Vec3 location = Vec3::lerp( 
		camera->transform->location,
		target_location,
		dt * smooth_move_speed
	);
	Quaternion rotation = Quaternion::lerp(
		camera->transform->rotation,
		target_rotation,
		dt * CAMERA_ROTATION_SPEED
	);
	camera->transform->set_location( location );
	camera->transform->set_rotation( rotation );

	//  update up direction for roll
	camera->up_direction = Vec3::lerp( 
		camera->up_direction, 
		up, 
		dt * CAMERA_ROTATION_SPEED 
	);

	//  update fov
	float fov = math::lerp( 
		CAMERA_FOV.x, 
		CAMERA_FOV.y, 
		throttle_ratio
	);
	/*float time = _game->get_timer()->get_accumulated_seconds();
	fov += throttle_ratio 
		 * math::sin( time * 5.0f )
		 * 1.0f;*/
	camera->set_fov( fov );
}
