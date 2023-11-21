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
	_handle_camera( dt );
	_handle_shoot( dt );
}

void PlayerSpaceship::_handle_movement( float dt )
{
	float time = _game->get_timer()->get_accumulated_seconds();
	auto inputs = _game->get_inputs();
	auto window = _game->get_window();

	//  get normalized mouse pos
	Vec2 window_size = window->get_size();
	Vec2 mouse_pos = inputs->get_mouse_pos();
	Vec2 normalized_mouse_pos = mouse_pos - window_size * 0.5f;
	normalized_mouse_pos /= window_size * 0.7f;
	normalized_mouse_pos *= -2.0f;
	normalized_mouse_pos.x = math::clamp( normalized_mouse_pos.x, -1.0f, 1.0f );
	normalized_mouse_pos.y = math::clamp( normalized_mouse_pos.y, -1.0f, 1.0f );
	//printf( "%f %f\n", normalized_mouse_pos.x, normalized_mouse_pos.y );

	//  find target location
	Vec3 target_spaceship_location = Vec3::forward * _previous_location;
	target_spaceship_location += Vec3::up * normalized_mouse_pos.y * 3.5f;
	target_spaceship_location += Vec3::right * -normalized_mouse_pos.x * 5.5f;

	//  lerp location to target
	_previous_location = Vec3::lerp(
		_previous_location,
		target_spaceship_location,
		dt * 5.0f
	);

	//  apply forward movement
	float move_speed = dt * 2.0f;
	Vec3 movement = Vec3::forward * move_speed;
	_previous_location += movement;

	//  apply idle location feeling
	Vec3 location = _previous_location;
	location += Vec3 {
		math::cos( time * 0.4f ) * 0.1f,
		math::sin( time * 1.8f ) * 0.3f,
		math::cos( time * 1.5f ) * 0.15f
	};

	//  apply location
	transform->set_location( location );

	//  aim velocity (movement feeling)
	const float MAX_AIM = 500.0f;
	_aim_velocity.x = math::clamp(
		_aim_velocity.x + inputs->mouse_delta.x,
		-MAX_AIM,
		MAX_AIM
	);
	_aim_velocity.y = math::clamp(
		_aim_velocity.y + inputs->mouse_delta.y,
		-MAX_AIM,
		MAX_AIM
	);
	_aim_velocity = Vec2::approach(
		_aim_velocity,
		Vec2::zero,
		dt * 200.0f
	);
	//printf( "%f %f\n", _aim_velocity.x, _aim_velocity.y );

	//  rotations
	Quaternion target_roll = Quaternion(
		Vec3::forward,
		-normalized_mouse_pos.x * math::PI * 0.05f
		+ -_aim_velocity.x / MAX_AIM * math::PI * 0.1f
	);
	Quaternion target_pitch = Quaternion(
		-Vec3::right,
		-normalized_mouse_pos.y * math::PI * 0.05f
		+ -_aim_velocity.y / MAX_AIM * math::PI * 0.1f
	);
	Quaternion rotation = transform->rotation;
	rotation = Quaternion::lerp(
		rotation,
		target_roll + target_pitch,
		dt * 15.0f
	);
	transform->set_rotation( rotation );
}

void PlayerSpaceship::_handle_camera( float dt )
{
	//  camera update
	camera->transform->set_location( 
		Vec3::forward * _previous_location - Vec3::forward * 7.0f 
	);
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
			+ Vec3::forward * 3.7f
			+ transform->get_right() * 2.0f * ( i == 0 ? 1.0f : -1.0f )
			+ transform->get_up() * 0.25f;
		projectile->transform->rotation = transform->rotation;
	}

	_shoot_time = SHOOT_TIME;
}
