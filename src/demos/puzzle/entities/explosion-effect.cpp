#include "explosion-effect.h"

#include <suprengine/easing.h>
#include <suprengine/random.h>

using namespace puzzle;

ExplosionEffect::ExplosionEffect( 
	float explosion_size,
	Color color
)
	: explosion_size( explosion_size ),
	  color( color ),
	  Entity()
{
	_model_renderer = create_component<StylizedModelRenderer>(
		Assets::get_model( "explosion" + std::to_string( random::generate( 0, 2 ) ) ),
		Color::white/*color*/
	);
	_model_renderer->inner_modulate = color/*Color::white*/;
	//_model_renderer->draw_only_outline = true;

	_max_lifetime = LIFETIME
		+ random::generate( -LIFETIME_DEVIATION, LIFETIME_DEVIATION );

	_lifetime_component = create_component<LifetimeComponent>( _max_lifetime );
	/*_lifetime_component->on_time_out.listen( "owner",
		std::bind( &ExplosionEffect::kill, this ) );*/

	_scale = Vec3 {
		random::generate( RANDOM_SCALE[0] ),
		random::generate( RANDOM_SCALE[1] ),
		random::generate( RANDOM_SCALE[2] ),
	};
	transform->rotation = random::generate_rotation();
}

void ExplosionEffect::update_this( float dt )
{
	const float lifetime = _max_lifetime - _lifetime_component->life_time;
	if ( lifetime < _max_lifetime )
	{
		//  compute scale over lifetime
		float scale_over_lifetime = 0.0f;
		if ( lifetime / LIFETIME <= FULL_SCALE_AT_LIFETIME_RATIO )
		{
			scale_over_lifetime = easing::out_expo( 
				lifetime / FULL_SCALE_AT_LIFETIME_RATIO );
		}
		else
		{
			//const float in_time = _max_lifetime * FULL_SCALE_AT_LIFETIME_RATIO;
			//const float ratio = ( lifetime - in_time ) / ( _max_lifetime - in_time );
			//scale_over_lifetime = 0.8f + 0.2f * easing::in_out_cubic( ratio );
			//scale_over_lifetime = 1.0f - easing::in_cubic( ratio );
			scale_over_lifetime = 1.0f;
		}

		//  lerp colors
		_model_renderer->modulate = Color::lerp(
			_model_renderer->modulate, color, dt * COLOR_OUTLINE_SPEED );
		_model_renderer->inner_modulate = Color::lerp( 
			_model_renderer->inner_modulate, Color::black, dt * COLOR_INNER_SPEED );
		
		//  progressively shrink outline
		_model_renderer->outline_scale = math::lerp( 
			OUTLINE_SCALE, 0.0f, lifetime / _max_lifetime );
	
		//  apply scale
		transform->set_scale( 
			  explosion_size 
			* scale_over_lifetime 
			* _scale
		);
	}
	else 
	{
		//  disable outline rendering
		_model_renderer->outline_scale = 0.0f;

		//  shrinking mesh
		transform->set_scale( 
			Vec3::lerp( 
				transform->scale, 
				Vec3::zero, 
				dt * LIFETIME_OUT_SHRINK_SPEED / explosion_size 
			) 
		);

		//  kill passing a threshold
		if ( transform->scale.x <= 0.05f )
		{
			kill();
			//printf( "killed explosion\n" );
		}
	}
}
