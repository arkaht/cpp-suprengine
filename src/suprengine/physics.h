#pragma once

#include "ray.hpp"

#include <suprengine/ecs/components/collider.fwd.h>

#include <vector>

namespace suprengine
{
	class Physics
	{
	private:
		std::vector<Collider*> colliders;

	public:
		void update();

		bool raycast( const Ray& ray, RayHit* hit );

		void add_collider( Collider* collider );
		void remove_collider( Collider* collider );
	};
}