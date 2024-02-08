#include "spaceship.h"

#include <entities/guided-missile.h>
#include <entities/explosion-effect.h>

#include <suprengine/random.h>

using namespace puzzle;

Spaceship::Spaceship()
{
	CameraDynamicDistanceSettings dcd_settings {};
	dcd_settings.is_active = true;
	dcd_settings.max_distance_sqr = math::pow( 256.0f, 2.0f );
	dcd_settings.max_outline_scale = 1.0f;

	//  setup components
	_model_renderer = create_component<StylizedModelRenderer>(
		Assets::get_model( "spaceship" ),
		_color
	);
	_model_renderer->dynamic_camera_distance_settings = dcd_settings;
	_model_renderer->outline_scale = MODEL_OUTLINE_SCALE;
	_collider = create_component<BoxCollider>( Box {
		Vec3 { -2.0f, -2.0f, -2.0f },
		Vec3 { 2.0f, 2.0f, 2.0f },
	} );

	//  initialize trail
	auto trail_entity = new Entity();
	_trail_renderer = trail_entity->create_component<StylizedModelRenderer>(
		_model_renderer->model,
		_color
	);
	_trail_renderer->dynamic_camera_distance_settings = dcd_settings;
	_trail_renderer->outline_scale = MODEL_OUTLINE_SCALE;
	_trail_renderer->draw_only_outline = true;
	_trail_renderer->draw_outline_ccw = false;

	//  health
	_health = create_component<HealthComponent>();
	_health->on_damage.listen( "owner", 
		std::bind( &Spaceship::_on_damage, this, std::placeholders::_1 ) );
}

Spaceship::~Spaceship()
{
	_trail_renderer->get_owner()->kill();

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
		projectile->damage_amount = 25.0f;
	}

	//  put on cooldown
	_shoot_time = SHOOT_TIME;
}

void Spaceship::launch_missiles( 
	std::weak_ptr<HealthComponent> wk_target 
)
{
	for ( int i = 0; i < 6; i++ )
	{
		float row = floorf( i / 2.0f );
		_engine->add_timer( { row * 0.1f, 
			[wk_target, this, i, row] {
				auto missile = new GuidedMissile( this, wk_target, _color );
				missile->transform->location = transform->location 
					+ transform->get_right() * ( i % 2 == 0 ? 1.0f : -1.0f ) * 2.0f
					+ transform->get_forward() * row * 3.0f;
				missile->transform->rotation = Quaternion::look_at( transform->get_up(), Vec3::up );
				missile->up_direction = transform->get_up();
			}
		} );
	}
}

void Spaceship::die()
{
	if ( _health->health > 0.0f )
	{
		_health->health = 0.0f;
	}

	_throttle = 0.0f;

	_model_renderer->is_active = false;
	_trail_renderer->is_active = false;
	_collider->is_active = false;

	state = EntityState::Paused;

	//  spawn explosion effect
	{
		float size_ratio_over_damage = math::clamp( 
			math::abs( _health->health / EXPLOSION_DAMAGE_FOR_MAX_SIZE ), 
			0.0f, 1.0f 
		);
		float size = math::lerp( EXPLOSION_SIZE.x, EXPLOSION_SIZE.y, size_ratio_over_damage );
		size += random::generate( EXPLOSION_SIZE_DEVIATION.x, EXPLOSION_SIZE_DEVIATION.y );

		auto effect = new ExplosionEffect( size, _color );
		effect->transform->location = transform->location;
	}
	printf( "Spaceship[%d] is killed!\n", get_unique_id() );

	TIMER( 5.0f, { respawn(); } );
}

void Spaceship::respawn()
{
	transform->set_location( Vec3::zero );
	transform->set_rotation( Quaternion::identity );

	_model_renderer->is_active = true;
	_trail_renderer->is_active = true;
	_collider->is_active = true;

	state = EntityState::Active;

	_health->heal_to_full();

	printf( "Spaceship[%d] has respawned!\n", get_unique_id() );
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
	_model_renderer->modulate = _color;
	_trail_renderer->modulate = _color;
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
	if ( controller )  //  avoid identity rotation when no controller
	{
		Quaternion rotation = inputs.should_smooth_rotation 
			? Quaternion::slerp( transform->rotation, inputs.desired_rotation, dt * inputs.smooth_rotation_speed )
			: inputs.desired_rotation;

		transform->set_rotation( rotation );
	}
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
	_trail_renderer->should_render = _trail_intensity > 0.01f;

	//  update visual
	if ( _trail_renderer->should_render )
	{
		auto& trail_transform = _trail_renderer->transform;

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
			   + _trail_renderer->outline_scale * TRAIL_MODEL_OUTLINE_SCALE_OFFSET;
		trail_transform->set_scale( scale );
	}
}

void Spaceship::_on_damage( const DamageResult& result )
{
	if ( !result.is_alive )
	{
		die();
	}
}
