#include "physics.h"

#include <suprengine/entity.h>
#include <suprengine/components/collider.h>

using namespace suprengine;

void Physics::update()
{
	for ( auto& collider : colliders )
	{
		//  Ignore a non-active collider
		if ( !collider->is_active ) continue;

		for ( auto& other : colliders )
		{
			//  Ignore self
			if ( collider == other ) continue;
			//  Ignore a non-active collider
			if ( !other->is_active ) continue;
			//  Ignore layers not in mask filter
			if ( ( collider->mask & other->get_owner()->layer ) == 0 ) continue;

			//  Update collisions between each other
			bool is_active = collider->intersects( other );
			collider->update_collision_with( other, is_active );
			other->update_collision_with( collider, is_active );
		}
	}
}

bool Physics::raycast( const Ray& ray, RayHit* hit, const RayParams& params )
{
	//  TODO: Add layer and mask to filter collision objects, probably a trace channel

	for ( auto& collider : colliders )
	{
		if ( !collider->is_active ) continue;
		if ( !collider->raycast( ray, hit, params ) ) continue;

		return true;
	}

	return false;
}

void Physics::add_collider( SharedPtr<Collider> collider )
{
	int order = collider->get_priority_order();

	//  Search order position
	auto itr = colliders.begin();
	for ( ; itr != colliders.end(); itr++ )
		if ( order >= (*itr)->get_priority_order() )
			break;

	colliders.insert( itr, collider );
}

void Physics::remove_collider( SharedPtr<Collider> collider )
{
	auto itr = std::find( colliders.begin(), colliders.end(), collider );
	if ( itr == colliders.end() ) return;

	colliders.erase( itr );  //  Don't swap or you need to sort again!
}