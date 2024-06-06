#pragma once
#include "collider.fwd.h"

#include <suprengine/component.h>
#include <suprengine/physics.h>
#include <suprengine/ray.hpp>

#include <unordered_set>

namespace suprengine
{
	enum class CollisionState
	{
		/*
		 * The collision just happened during this update.
		 */
		Enter,
		/*
		 * The collision is still happening during this update.
		 */
		Stay,
		/*
		 * The collision just stopped during this update.
		 */
		Exit,
	};

	class Collider : public Component
	{
	public:
		Collider( int priority_order = 0 );

		void setup();
		void unsetup();

		virtual bool intersects( shared_ptr<Collider> other ) = 0;
		virtual bool raycast( _RAYCAST_FUNC_PARAMS ) = 0;

		void update_collision_with( shared_ptr<Collider> other, bool active );
	
	public:
		uint32_t mask = 0xFFFFFFFF;

		Color debug_color { 0, 255, 0, 127 };

	public:
		/*
		 * Called when a collision with another collider is updated.
		 * 
		 * Parameters:
		 * - shared_ptr<Collider> other_collider
		 * - CollisionState state
		 */
		Event<shared_ptr<Collider>, CollisionState> on_collision_update;

	protected:
		std::unordered_set<shared_ptr<Collider>> collisions;
	};
}