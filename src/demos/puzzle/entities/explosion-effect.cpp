#include "explosion-effect.h"

#include <suprengine/easing.h>
#include <suprengine/random.h>

using namespace puzzle;

ExplosionEffect::ExplosionEffect( 
	float explosion_size,
	Color color
)
	: explosion_size( explosion_size )
{
	_model_renderer = create_component<StylizedModelRenderer>(
		Assets::get_model( suprengine::MESH_SPHERE ),
		color
	);

	_max_lifetime = LIFETIME
		+ random::generate( -LIFETIME_DEVIATION, LIFETIME_DEVIATION );

	_lifetime_component = create_component<LifetimeComponent>( _max_lifetime );
	_lifetime_component->on_time_out.listen( "owner",
		std::bind( &ExplosionEffect::kill, this ) );
}

void ExplosionEffect::update_this( float dt )
{
	//  compute scale over lifetime
	const float lifetime = _max_lifetime - _lifetime_component->life_time;
	float scale_over_lifetime = 0.0f;
	if ( lifetime / LIFETIME <= FULL_SCALE_AT_LIFETIME_RATIO )
	{
		scale_over_lifetime = easing::out_expo( 
			lifetime / FULL_SCALE_AT_LIFETIME_RATIO );
	}
	else
	{
		const float in_time = _max_lifetime * FULL_SCALE_AT_LIFETIME_RATIO;
		scale_over_lifetime = 1.0f - easing::in_cubic( 
			( lifetime - in_time ) / ( _max_lifetime - in_time ) );
	}

	//  output final scale
	transform->set_scale( 
		  explosion_size 
		* scale_over_lifetime 
		* Vec3::one 
	);
}
