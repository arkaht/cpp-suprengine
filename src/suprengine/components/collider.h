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
		Collider( Entity* owner, int priority_order = 0 );

		void setup();
		void unsetup();

		virtual bool intersects( std::shared_ptr<Collider> other ) = 0;
		virtual bool raycast( _RAYCAST_FUNC_PARAMS ) = 0;

		void update_collision_with( std::shared_ptr<Collider> other, bool active );
	
	public:
		uint32_t mask = 0xFFFFFFFF;

		Color debug_color { 0, 255, 0, 127 };

	protected:
		Physics* _physics;
		std::unordered_set<std::shared_ptr<Collider>> collisions;
	};
}