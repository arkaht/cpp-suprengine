#include "collider.h"
#include "../entity.h"

using namespace suprengine;

Collider::Collider( int priority_order )
	: Component( priority_order )
{}

void Collider::setup()
{
	auto physics = Engine::instance().get_physics();

	auto ptr = get_shared_from_this<Collider>();
	get_owner()->collider = ptr;
	physics->add_collider( ptr );
}

void Collider::unsetup()
{
	auto physics = Engine::instance().get_physics();

	auto ptr = get_shared_from_this<Collider>();
	physics->remove_collider( ptr );
}

void Collider::update_collision_with( shared_ptr<Collider> other, bool is_active )
{
	bool is_found = collisions.find( other ) != collisions.end();

	if ( is_active )
	{
		if ( !is_found )
		{
			collisions.emplace( other );
			get_owner()->on_trigger_enter( other );
		}
		else
		{
			get_owner()->on_trigger_stay( other );
		}
	}
	else
	{
		if ( is_found )
		{
			collisions.erase( other );
			get_owner()->on_trigger_exit( other );
		}
	}
}
