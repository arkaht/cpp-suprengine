#include "player-spaceship.h"

#include <entities/projectile.h>

using namespace puzzle;

PlayerSpaceship::PlayerSpaceship()
{
	//  setup components
	model_renderer = create_component<StylizedModelRenderer>(
		Assets::get_model( "spaceship" ),
		_color
	);

	//  initialize camera
	camera_owner = new Entity();
	camera = camera_owner->create_component<Camera>( 77.7f );
	camera->activate();
}

void PlayerSpaceship::update_this( float dt )
{
	_handle_movement( dt );
	_handle_shoot( dt );
	_handle_camera( dt );
}

void PlayerSpaceship::_handle_movement( float dt )
{
	float time = _game->get_timer()->get_accumulated_seconds();
	auto inputs = _game->get_inputs();
	auto window = _game->get_window();

	const Vec3 AIM_SENSITIVITY { 0.3f, 0.1f, 0.075f };
	const float MAX_AIM_VELOCITY = 2.0f;
	const float MAX_THROTTLE_SPEED = 200.0f;

	//  get inputs
	Vec2 mouse_delta = -inputs->mouse_delta;
	float yaw_delta = inputs->get_keys_as_axis( SDL_SCANCODE_A, SDL_SCANCODE_D, AIM_SENSITIVITY.z );
	float throttle_delta = inputs->get_keys_as_axis( SDL_SCANCODE_S, SDL_SCANCODE_W, 0.2f );

	//  throttle
	_throttle = math::clamp( _throttle + throttle_delta * dt, 0.1f, 1.0f );

	//  handle aim velocity
	{
		//  add inputs
		_aim_velocity.x = math::clamp( 
			_aim_velocity.x + mouse_delta.x * AIM_SENSITIVITY.x, 
			-MAX_AIM_VELOCITY, 
			MAX_AIM_VELOCITY 
		);
		_aim_velocity.y = math::clamp( 
			_aim_velocity.y + mouse_delta.y * AIM_SENSITIVITY.y, 
			-MAX_AIM_VELOCITY, 
			MAX_AIM_VELOCITY 
		);
		_aim_velocity.z = math::clamp(
			_aim_velocity.z + yaw_delta,
			-MAX_AIM_VELOCITY,
			MAX_AIM_VELOCITY
		);

		//  drag to zero
		_aim_velocity = Vec3::lerp( _aim_velocity, Vec2::zero, dt * 5.0f );
	}

	//  apply forward movement
	float move_speed = dt * _throttle * MAX_THROTTLE_SPEED;
	Vec3 movement = transform->get_forward() * move_speed;
	_previous_location += movement;
	transform->set_location( _previous_location );

	//  apply rotation
	Quaternion rotation = transform->rotation;
	rotation = rotation + Quaternion( 
		transform->get_forward(), 
		_aim_velocity.x * dt 
	);
	rotation = rotation + Quaternion( 
		transform->get_right(), 
		_aim_velocity.y * dt 
	);
	rotation = rotation + Quaternion(
		transform->get_up(),
		_aim_velocity.z * dt
	);
	transform->set_rotation( rotation );
}

void PlayerSpaceship::_handle_camera( float dt )
{
	auto inputs = _game->get_inputs();

	Vec3 forward = transform->get_forward() * -7.0f;
	if ( inputs->is_key_down( SDL_SCANCODE_E ) )
	{
		forward *= -3.0f;
	}

	//  apply location
	Vec3 target_location = transform->location 
	  + forward
	  + transform->get_up() * 2.5f;
	Vec3 location = Vec3::lerp( 
		camera->transform->location, 
		target_location, 
		dt * 5.0f 
	);
	camera->transform->set_location( location );

	//  apply rotation
	Quaternion rotation = transform->rotation;
	if ( inputs->is_key_down( SDL_SCANCODE_E ) )
	{
		rotation = Quaternion::look_at( -transform->get_right(), transform->get_up() );
	}
	camera->transform->set_rotation( rotation );

	//  update up direction for roll
	camera->up_direction = transform->get_up();
}

void PlayerSpaceship::_handle_shoot( float dt )
{
	_shoot_time -= dt;

	auto inputs = _game->get_inputs();
	if ( !inputs->is_key_down( SDL_SCANCODE_SPACE ) ) return;
	if ( _shoot_time > 0.0f ) return;

	for ( int i = 0; i < 2; i++ )
	{
		auto projectile = new Projectile( _color );
		projectile->transform->location = transform->location
			+ transform->get_forward() * 3.7f
			+ transform->get_right() * 2.0f * ( i == 0 ? 1.0f : -1.0f )
			+ transform->get_up() * 0.25f;
		projectile->transform->rotation = transform->rotation;
	}

	_shoot_time = SHOOT_TIME;
}
