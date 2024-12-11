#include "physics.h"

#include <suprengine/entity.h>
#include <suprengine/components/collider.h>

using namespace suprengine;

void Physics::update()
{
	//std::unordered_map<Collider*, std::unordered_set<Collider*>> checked_colliders;
	for ( auto& collider : colliders )
	{
		//  ignore non-active collider
		if ( !collider->is_active ) continue;

		for ( auto& other : colliders )
		{
			//  ignore self
			if ( collider == other ) continue;
			//  ignore non-active collider
			if ( !other->is_active ) continue;
			//  ignore layers not in mask filter
			if ( ( collider->mask & other->get_owner()->layer ) == 0 ) continue;

			//  ignore checked colliders
			/*if ( checked_colliders.find( other ) != checked_colliders.end() )
			{
				auto& set = checked_colliders[other];
				if ( set.find( collider ) != set.end() )
					continue;
			}*/

			//  collision?
			bool is_active = collider->intersects( other );

			//  update
			collider->update_collision_with( other, is_active );
			other->update_collision_with( collider, is_active );

			//  checked
			/*checked_colliders[collider].emplace( other );
			checked_colliders[other].emplace( collider );*/
		}
	}
}

bool Physics::raycast( const Ray& ray, RayHit* hit, const RayParams& params )
{
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
	//  get priority order
	int order = collider->get_priority_order();

	//  search order position
	auto itr = colliders.begin();
	for ( ; itr != colliders.end(); itr++ )
		if ( order >= ( *itr )->get_priority_order() )
			break;

	colliders.push_back( collider );
}

void Physics::remove_collider( SharedPtr<Collider> collider )
{
	auto itr = std::find( colliders.begin(), colliders.end(), collider );
	if ( itr == colliders.end() ) return;

	colliders.erase( itr );  //  don't swap or you need to sort again!
}