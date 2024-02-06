#include "lifetime-component.h"

using namespace suprengine;

LifetimeComponent::LifetimeComponent( 
	Entity* owner, 
	float life_time 
)
	: life_time( life_time ),
	  Component( owner )
{}

void LifetimeComponent::update( float dt )
{
	if ( life_time <= 0.0f ) return;

	life_time -= dt;
	if ( life_time > 0.0f ) return;
	
	on_time_out.invoke();
}
