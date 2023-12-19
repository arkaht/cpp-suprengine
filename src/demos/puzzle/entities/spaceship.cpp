#include "spaceship.h"

using namespace puzzle;

Spaceship::Spaceship()
{
	DynamicCameraDistanceSettings dcd_settings {};
	dcd_settings.is_active = true;
	dcd_settings.max_distance_sqr = math::pow( 256.0f, 2.0f );
	dcd_settings.max_outline_scale = 1.0f;

	//  setup components
	model_renderer = create_component<StylizedModelRenderer>(
		Assets::get_model( "spaceship" ),
		_color
	);
	model_renderer->dynamic_camera_distance_settings = dcd_settings;
	model_renderer->outline_scale = MODEL_OUTLINE_SCALE;
	create_component<BoxCollider>( Box {
		Vec3 { -2.0f, -2.0f, -2.0f },
		Vec3 { 2.0f, 2.0f, 2.0f },
	} );

	//  initialize trail
	auto trail_entity = new Entity();
	trail_renderer = trail_entity->create_component<StylizedModelRenderer>(
		model_renderer->model,
		_color
	);
	trail_renderer->dynamic_camera_distance_settings = dcd_settings;
	trail_renderer->outline_scale = MODEL_OUTLINE_SCALE;
	trail_renderer->draw_only_outline = true;
	trail_renderer->draw_outline_ccw = false;
}

Spaceship::~Spaceship()
{
	trail_renderer->get_owner()->kill();

	if ( controller )
	{
		controller->unpossess();
	}
}

void Spaceship::update_this( float dt )
{
	_update_movement( dt );
	_update_trail( dt );

	//  reduce shoot cooldown
	_shoot_time = math::max( 0.0f, _shoot_time - dt );
}

void Spaceship::shoot()
{
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

Vec3 Spaceship::get_shoot_location( const Vec3& axis_scale ) const
{
	return transform->location
		+ transform->get_forward() * 3.7f * axis_scale.x
		+ transform->get_right() * 2.0f * axis_scale.y
		+ transform->get_up() * 0.25f * axis_scale.z;
}

void Spaceship::set_color( const Color& color )
{
	_color = color;

	//  update renderers
	model_renderer->modulate = _color;
	trail_renderer->modulate = _color;
}

void Spaceship::_update_movement( float dt )
{
	//  get inputs
	auto inputs = SpaceshipControlInputs {};
	if ( controller )
	{
		controller->update_inputs( dt );
		inputs = controller->get_inputs();
	}

	float throttle_delta = inputs.throttle_delta;
	float throttle_speed = THROTTLE_GAIN_SPEED;

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
		dt * throttle_speed
	);
	//printf( "%.0f%%\n", _throttle * 100.0f );

	//  apply forward movement
	float move_speed = dt * _throttle * MAX_THROTTLE_SPEED;
	Vec3 movement = transform->get_forward() * move_speed;
	transform->set_location( transform->location + movement );

	//  apply rotation
	Quaternion rotation = transform->rotation;
	if ( controller )  //  avoid identiy rotation when no controller
	{
		rotation = inputs.desired_rotation;
	}
	transform->set_rotation( rotation );
}

void Spaceship::_update_trail( float dt )
{
	float time = _engine->get_timer()->get_accumulated_seconds();

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
			   + trail_renderer->outline_scale * TRAIL_MODEL_OUTLINE_SCALE_OFFSET;
		trail_transform->set_scale( scale );
	}
}
