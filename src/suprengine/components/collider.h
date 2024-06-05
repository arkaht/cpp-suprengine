#pragma once
#include "collider.fwd.h"

#include <suprengine/component.h>
#include <suprengine/physics.h>
#include <suprengine/ray.hpp>

#include <unordered_set>

namespace suprengine
{
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

	protected:
		std::unordered_set<shared_ptr<Collider>> collisions;
	};
}