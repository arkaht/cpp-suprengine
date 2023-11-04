#pragma once

#include "ray.hpp"

#include <suprengine/components/collider.fwd.h>

#include <vector>
#include <memory>

namespace suprengine
{
	class Physics
	{
	public:
		void update();

		bool raycast( _RAYCAST_FUNC_PARAMS );

		void add_collider( std::shared_ptr<Collider> collider );
		void remove_collider( std::shared_ptr<Collider> collider );
	
	private:
		std::vector<std::shared_ptr<Collider>> colliders;
	};
}