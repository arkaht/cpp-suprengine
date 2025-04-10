#pragma once

#include <suprengine/core/component.h>

#include <suprengine/math/color.h>

#include <suprengine/utils/ray.h>
#include <suprengine/utils/event.h>

#include <set>

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

		virtual bool intersects( SharedPtr<Collider> other ) = 0;
		virtual bool raycast( const Ray& ray, RayHit* hit, const RayParams& params ) = 0;

		void update_collision_with( SharedPtr<Collider> other, bool active );
	
	public:
		uint32_t mask = 0xFFFFFFFF;

		Color debug_color { 0, 255, 0, 127 };

	public:
		/*
		 * Called when a collision with another collider is updated.
		 * 
		 * Parameters:
		 * - SharedPtr<Collider> other_collider
		 * - CollisionState state
		 */
		Event<SharedPtr<Collider>, CollisionState> on_collision_update;

	protected:
		std::set<Collider*> collisions;
	};
}