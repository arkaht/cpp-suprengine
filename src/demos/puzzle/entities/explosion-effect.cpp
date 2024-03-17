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

	//  get curves
	_curve_scale_over_time = Assets::get_curve( "explosion-scale-over-time" );
}

void ExplosionEffect::update_this( float dt )
{
	const float lifetime = _max_lifetime - _lifetime_component->life_time;
	if ( lifetime < _max_lifetime )
	{
		//  evaluate curve
		float t = lifetime / _max_lifetime;
		float scale_over_lifetime = 
			_curve_scale_over_time->evaluate_by_time( t );

		//  lerp colors
		_model_renderer->modulate = Color::lerp(
			_model_renderer->modulate, 
			color, 
			dt * COLOR_OUTLINE_SPEED 
		);
		_model_renderer->inner_modulate = Color::lerp( 
			_model_renderer->inner_modulate, 
			Color::black, 
			dt * COLOR_INNER_SPEED 
		);
		
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
