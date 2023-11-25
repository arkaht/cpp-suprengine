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

	//  initialize trail
	auto trail_entity = new Entity();
	trail_renderer = trail_entity->create_component<StylizedModelRenderer>(
		model_renderer->model,
		_color
	);
	trail_renderer->draw_only_outline = true;
	trail_renderer->draw_outline_ccw = false;

	CameraProjectionSettings projection_settings {};
	projection_settings.znear = 1.0f;
	projection_settings.zfar = 10000.0f;

	//  initialize camera
	auto camera_owner = new Entity();
	camera = camera_owner->create_component<Camera>( projection_settings );
	camera->activate();

	camera_owner = new Entity();
	camera_owner->transform->scale = Vec3( 10.0f );
	second_camera = camera_owner->create_component<Camera>( projection_settings );
	camera_owner->create_component<ModelRenderer>( Assets::get_model( "projectile" ) );
}

PlayerSpaceship::~PlayerSpaceship()
{
	camera->get_owner()->kill();
	second_camera->get_owner()->kill();
	trail_renderer->get_owner()->kill();
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
	float throttle_speed = 1.0f + /*inputs->is_key_down( SDL_SCANCODE_LSHIFT ) **/ 1.5f;

	//  throttle
	_throttle = math::clamp( _throttle + throttle_delta * throttle_speed * dt, 0.0f, 1.0f );
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

	//  trail
	{
		//  intensity
		const float trail_throttle_start = 0.3f;
		const float trail_intensity_offset = 0.6f;
		_trail_intensity = math::lerp( 
			_trail_intensity, 
			_throttle > trail_throttle_start 
				? math::remap( _throttle, trail_throttle_start, 1.0f, 0.0f, 1.0f )
				: 0.0f,
			2.0f * dt
		);
		trail_renderer->should_render = _trail_intensity > 0.01f;

		//  update visual
		if ( trail_renderer->should_render )
		{
			auto trail_transform = trail_renderer->transform;
			trail_transform->set_location( 
				transform->location 
			  + -transform->get_forward() 
				* _trail_intensity
				* math::abs( 0.1f + math::sin( time * _throttle * 15.0f ) * 0.25f )
			);
			trail_transform->set_rotation( transform->rotation );
			trail_transform->set_scale( 
				transform->scale 
			  * ( 1.0f * math::min( trail_intensity_offset + _trail_intensity, 1.0f ) + trail_renderer->outline_scale * 2.0f ) 
			  * Vec3 { 1.00f, 1.0f, 0.5f }
			);
		}
	}
}

void PlayerSpaceship::_handle_camera( float dt )
{
	auto inputs = _game->get_inputs();

	const Vec2 CAMERA_BACKWARD_RANGE { 6.0f, -3.0f };
	const Vec2 CAMERA_SPEED_RANGE { 7.0f, 12.0f };
	const Vec2 CAMERA_UP_RANGE { 2.0f, 4.0f };

	float throttle_ratio = _throttle / 1.0f;
	float smooth_speed = math::lerp( CAMERA_SPEED_RANGE.x, CAMERA_SPEED_RANGE.y, throttle_ratio );
	float backward_distance = math::lerp( CAMERA_BACKWARD_RANGE.x, CAMERA_BACKWARD_RANGE.y, throttle_ratio );
	float up_distance = math::lerp( CAMERA_UP_RANGE.x, CAMERA_UP_RANGE.y, throttle_ratio );

	if ( inputs->is_mouse_button_just_pressed( MouseButton::Right ) )
	{
		auto next_camera = camera->is_active() ? second_camera : camera;
		next_camera->activate();
	}

	if ( second_camera->is_active() )
	{
		//second_camera->up_direction = transform->get_up();
	}
	Quaternion rot = Quaternion::look_at( 
		Vec3::cross( ( second_camera->transform->location - transform->location ).normalized(), Vec3::up ), 
		Vec3::up
	);
	second_camera->transform->set_rotation( 
		rot
	);

	Vec3 forward = transform->get_forward() * -backward_distance;
	/*if ( inputs->is_key_down( SDL_SCANCODE_E ) )
	{
		forward *= -3.0f;
	}*/

	//  apply location
	Vec3 target_location = transform->location 
	  + forward
	  + transform->get_up() * up_distance;
	Vec3 location = Vec3::lerp( 
		camera->transform->location,
		target_location,
		dt * smooth_speed
	);
	camera->transform->set_location( location );

	//  apply rotation
	Quaternion rotation = transform->rotation;
	/*if ( inputs->is_key_down( SDL_SCANCODE_E ) )
	{
		rotation = Quaternion::look_at( -transform->get_right(), transform->get_up() );
	}*/
	camera->transform->set_rotation( rotation );

	//  update up direction for roll
	camera->up_direction = Vec3::lerp( camera->up_direction, transform->get_up(), dt * smooth_speed );
}

void PlayerSpaceship::_handle_shoot( float dt )
{
	_shoot_time -= dt;

	auto inputs = _game->get_inputs();
	if ( !inputs->is_mouse_button_down( MouseButton::Left ) ) return;
	if ( _shoot_time > 0.0f ) return;

	for ( int i = 0; i < 2; i++ )
	{
		auto projectile = new Projectile( _color );
		projectile->transform->scale = Vec3( 1.5f );
		projectile->transform->location = transform->location
			+ transform->get_forward() * 3.7f * projectile->transform->scale.x
			+ transform->get_right() * 2.0f * ( i == 0 ? 1.0f : -1.0f )
			+ transform->get_up() * 0.25f;
		projectile->transform->rotation = transform->rotation;
	}

	_shoot_time = SHOOT_TIME;
}
