#pragma once

#include <suprengine/utils/ray.h>
#include <suprengine/utils/memory.h>

#include <vector>

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