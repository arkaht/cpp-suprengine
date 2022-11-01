#include "collider.h"
#include "../entity.h"

using namespace suprengine;

Collider::Collider( Entity* owner, int priority_order )
	: Component( owner, priority_order )
{
	owner->collider = this;
	owner->get_game()->add_collider( this );
}

Collider::~Collider()
{
	owner->get_game()->remove_collider( this );
}

void Collider::update_collision_with( Collider* other, bool is_active )
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
