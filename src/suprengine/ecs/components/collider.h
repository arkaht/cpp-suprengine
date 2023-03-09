#pragma once
#include "collider.fwd.h"
#include "../component.h"

#include <suprengine/ray.hpp>

#include <unordered_set>

namespace suprengine
{
	class Collider : public Component
	{
	protected:
		std::unordered_set<Collider*> collisions;
	public:
		uint32_t mask = 0xFFFFFFFF;

		Color debug_color { 0, 255, 0, 127 };

		Collider( Entity* owner, int priority_order = 0 );
		~Collider() override;

		virtual bool intersects( Collider* other ) = 0;
		virtual bool raycast( const Ray& ray, RayHit* hit ) = 0;

		void update_collision_with( Collider* other, bool active );
	};
}