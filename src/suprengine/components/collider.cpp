#include "collider.h"
#include "../entity.h"

using namespace suprengine;

Collider::Collider( Entity* owner, int priority_order )
	: Component( owner, priority_order )
{
	_physics = owner->get_engine()->get_physics();
}

void Collider::setup()
{
	auto ptr = get_shared_from_this<Collider>();
	owner->collider = ptr;
	_physics->add_collider( ptr );
}

void Collider::unsetup()
{
	auto ptr = get_shared_from_this<Collider>();
	_physics->remove_collider( ptr );
}

void Collider::update_collision_with( std::shared_ptr<Collider> other, bool is_active )
{
	bool is_found = collisions.find( other ) != collisions.end();

	if ( is_active )
	{
		if ( !is_found )
		{
			collisions.emplace( other );
			owner->on_trigger_enter( other );
		}
		else
		{
			owner->on_trigger_stay( other );
		}
	}
	else
	{
		if ( is_found )
		{
			collisions.erase( other );
			owner->on_trigger_exit( other );
		}
	}
}
