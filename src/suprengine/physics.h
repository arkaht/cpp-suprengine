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

		bool raycast( _RAYCAST_FUNC_PARAMS );

		void add_collider( Collider* collider );
		void remove_collider( Collider* collider );
	};
}