#pragma once

#include <suprengine/vec3.h>
#include <suprengine/components/collider.fwd.h>

#include <memory>

#define _RAYCAST_FUNC_PARAMS const Ray& ray, RayHit* hit, const RayParams& params

namespace suprengine
{
	struct Ray
	{
		Vec3 origin = Vec3::zero;
		Vec3 direction = Vec3::zero;
		float distance = 0.0f;

		Ray( const Vec3& origin, const Vec3& direction, float distance )
			: origin( origin ), direction( direction ), distance( distance ) {}

		Ray( const Vec3& origin, const Vec3& dir )
			: origin( origin ) 
		{
			distance = dir.length();
			if ( distance == 0.0f ) return;

			direction = Vec3 { dir.x / distance, dir.y / distance, dir.z / distance };  //  normalize direction
		}
	};

	struct RayHit
	{
		Vec3 point = Vec3::zero;
		Vec3 normal = Vec3::zero;
		std::weak_ptr<Collider> collider;
	};

	struct RayParams
	{
		bool can_hit_from_origin = true;
		//  TODO: add collision layer
	};
}