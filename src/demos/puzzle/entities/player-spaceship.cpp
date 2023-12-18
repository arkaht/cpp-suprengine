#include "player-spaceship.h"

#include <suprengine/components/colliders/box-collider.h>
#include <suprengine/easing.h>

#include <entities/projectile.h>
#include <components/player-hud.h>

using namespace puzzle;

PlayerSpaceship::PlayerSpaceship()
{
	//  setup components
	model_renderer = create_component<StylizedModelRenderer>(
		Assets::get_model( "spaceship" ),
		_color
	);
	create_component<BoxCollider>( Box::HALF );
	create_component<PlayerHUD>( this, _color );

	//  initialize trail
	auto trail_entity = new Entity();
	trail_renderer = trail_entity->create_component<StylizedModelRenderer>(
		model_renderer->model,
		_color
	);
	trail_renderer->draw_only_outline = true;
	trail_renderer->draw_outline_ccw = false;

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
	second_camera = camera_owner->create_component<Camera>( projection_settings );
	//camera_owner->create_component<ModelRenderer>( Assets::get_model( "projectile" ) );
}

PlayerSpaceship::~PlayerSpaceship()
{
	camera->get_owner()->kill();
	second_camera->get_owner()->kill();
	trail_renderer->get_owner()->kill();
}

void PlayerSpaceship::update_this( float dt )
{
	_update_movement( dt );
	_update_shoot( dt );
	_update_camera( dt );
}

Vec3 PlayerSpaceship::get_shoot_location( const Vec3& axis_scale ) const
{
	return transform->location
		+ transform->get_forward() * 3.7f * axis_scale.x
		+ transform->get_right() * 2.0f * axis_scale.y
		+ transform->get_up() * 0.25f * axis_scale.z;
}

void PlayerSpaceship::_update_movement( float dt )
{
	float time = _engine->get_timer()->get_accumulated_seconds();
	auto inputs = _engine->get_inputs();
	auto window = _engine->get_window();

	//  get mouse delta
	Vec2 mouse_delta = -inputs->mouse_delta;
	//printf( "x=%f; y=%f\n", mouse_delta.x, mouse_delta.y );
	//mouse_delta.x *= easing::in_sine( math::abs( mouse_delta.x ) / max_mouse_delta );
	//mouse_delta.y *= easing::in_sine( math::abs( mouse_delta.y ) / max_mouse_delta );
	
	//  get inputs
	float yaw_delta = inputs->get_keys_as_axis( 
		SDL_SCANCODE_A, SDL_SCANCODE_D, AIM_SENSITIVITY.z );
	float throttle_delta = inputs->get_keys_as_axis( 
		SDL_SCANCODE_S, SDL_SCANCODE_W, 1.0f );
	float throttle_speed = THROTTLE_GAIN_SPEED 
		/*+ inputs->is_key_down( SDL_SCANCODE_LSHIFT ) * 1.5f*/;

	//  find throttle target
	float throttle_target = math::clamp( _throttle, 0.0f, 1.0f );
	if ( throttle_delta > 0.0f )
	{
		throttle_target = 1.0f + MAX_THROTTLE_FORWARD_OFFSET;
	}
	else if ( throttle_delta < 0.0f )
	{
		throttle_target = 0.0f;
	}

	//  throttle
	_throttle = math::approach( 
		_throttle,
		throttle_target,
		throttle_speed * dt
	);
	//printf( "%.0f%%\n", _throttle * 100.0f );

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
		_aim_velocity = Vec3::lerp( 
			_aim_velocity, 
			Vec2::zero, 
			dt * AIM_VELOCITY_DECREASE
		);
	}

	//  apply forward movement
	float move_speed = dt * _throttle * MAX_THROTTLE_SPEED;
	Vec3 movement = transform->get_forward() * move_speed;
	transform->set_location( transform->location + movement );

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

	//  trail
	_update_trail( dt, time );
}

void PlayerSpaceship::_update_trail( float dt, float time )
{
	//  intensity
	float trail_intensity_target = 0.0f;
	if ( _throttle > TRAIL_THROTTLE_START )
	{
		trail_intensity_target = math::remap( 
			_throttle, 
			TRAIL_THROTTLE_START, 1.0f, 
			0.0f, 1.0f 
		);
	}
	_trail_intensity = math::lerp(
		_trail_intensity,
		trail_intensity_target,
		dt * TRAIL_INTENSITY_SPEED
	);
	trail_renderer->should_render = _trail_intensity > 0.01f;

	//  update visual
	if ( trail_renderer->should_render )
	{
		auto& trail_transform = trail_renderer->transform;

		//  location
		Vec3 location = transform->location;
		location += -transform->get_forward()
			* _trail_intensity
			* math::abs( 0.1f + math::sin( time * _throttle * TRAIL_WAVE_FREQUENCY ) * TRAIL_WAVE_AMPLITUDE );
		trail_transform->set_location( location );

		//  rotation
		trail_transform->set_rotation( transform->rotation );

		//  scale
		Vec3 scale = transform->scale * TRAIL_MODEL_SCALE;
		scale *= math::min( TRAIL_MODEL_SCALE_INTENSITY_OFFSET + _trail_intensity, 1.0f ) 
			   + trail_renderer->outline_scale * TRAIL_MODEL_OUTLINE_SCALE;
		trail_transform->set_scale( scale );
	}
}

void PlayerSpaceship::_update_camera( float dt )
{
	auto inputs = _engine->get_inputs();

	float throttle_ratio = easing::in_out_cubic( _throttle / 1.0f );
	float smooth_move_speed = math::lerp( 
		CAMERA_MOVE_SPEED.x, CAMERA_MOVE_SPEED.y, throttle_ratio );
	float up_distance = math::lerp( 
		CAMERA_UP_RANGE.x, CAMERA_UP_RANGE.y, throttle_ratio );

	//if ( inputs->is_mouse_button_just_pressed( MouseButton::Right ) )
	//{
	//	auto& next_camera = camera->is_active() ? second_camera : camera;
	//	next_camera->activate();
	//}

	//if ( second_camera->is_active() )
	//{
	//	//second_camera->up_direction = transform->get_up();
	//}

	//Vec3 dir = ( transform->location - second_camera->transform->location ).normalized();
	//Quaternion rot = Quaternion::look_at( 
	//	/*Vec3::cross( 
	//		dir,
	//		Vec3::up 
	//	)*/dir, 
	//	Vec3::up
	//);
	//rot.normalize();
	//second_camera->transform->set_rotation( rot );
	
	const Vec3 up = transform->get_up();
	const Vec3 forward = transform->get_forward();
	/*if ( inputs->is_key_down( SDL_SCANCODE_E ) )
	{
		forward *= -3.0f;
	}*/
	
	//  find targets location & rotation
	Vec3 target_location = transform->location
	  + up * up_distance;
	Quaternion target_rotation = transform->rotation;
	if ( inputs->is_mouse_button_down( MouseButton::Right ) )
	{
		float distance = math::lerp( 
			CAMERA_LOOK_BACKWARD_DISTANCE.x, 
			CAMERA_LOOK_BACKWARD_DISTANCE.y, 
			_throttle  //  looks better on linearly progressive distance
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

void PlayerSpaceship::_update_shoot( float dt )
{
	auto inputs = _engine->get_inputs();

	//  reduce shoot cooldown
	_shoot_time = math::max( 0.0f, _shoot_time - dt );

	if ( _shoot_time > 0.0f ) return;
	if ( !inputs->is_mouse_button_down( MouseButton::Left ) ) return;

	//  spawn projectile
	for ( int i = 0; i < 2; i++ )
	{
		auto projectile = new Projectile( this, _color );
		projectile->transform->scale = Vec3( 1.5f );
		projectile->transform->location = 
			get_shoot_location( 
				Vec3 {
					projectile->transform->scale.x,
					i == 0 ? -1.0f : 1.0f,
					1.0f
				} 
			);
		projectile->transform->rotation = transform->rotation;
	}

	//  put on cooldown
	_shoot_time = SHOOT_TIME;
}
