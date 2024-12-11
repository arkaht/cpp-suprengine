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

		bool raycast( const Ray& ray, RayHit* hit, const RayParams& params );

		void add_collider( SharedPtr<Collider> collider );
		void remove_collider( SharedPtr<Collider> collider );
	
	private:
		std::vector<SharedPtr<Collider>> colliders;
	};
}