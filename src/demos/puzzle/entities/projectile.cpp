#include "projectile.h"

using namespace puzzle;

Projectile::Projectile( Color color )
	: _color( color )
{
	model_renderer = create_component<StylizedModelRenderer>(
		Assets::get_model( "projectile" ),
		color
	);
	model_renderer->draw_only_outline = true;
}

void Projectile::update_this( float dt )
{
	//  movement
	transform->set_location(
		transform->location
	  + transform->get_forward() * move_speed * dt
	);

	//  life time
	life_time -= dt;
	if ( life_time <= 0.0f )
	{
		kill();
	}
}
