#pragma once

#include <suprengine/usings.h>
#include <suprengine/vec3.h>
#include <suprengine/components/collider.fwd.h>

#define _RAYCAST_FUNC_PARAMS const Ray& ray, RayHit* hit, const RayParams& params

namespace suprengine
{
	struct Ray
	{
	public:
		Vec3 origin = Vec3::zero;
		Vec3 direction = Vec3::zero;
		float distance = 0.0f;

	public:
		Ray( 
			const Vec3& origin, 
			const Vec3& direction, 
			float distance 
		)
			: origin( origin ), 
			  direction( direction ), 
			  distance( distance ) 
		{}

		Ray( const Vec3& origin, const Vec3& translation )
			: origin( origin ) 
		{
			distance = translation.length();
			if ( distance == 0.0f ) return;

			//  normalize direction
			direction = Vec3 { 
				translation.x / distance, 
				translation.y / distance, 
				translation.z / distance 
			};
		}
	};

	struct RayHit
	{
		/*
		 * Hit location of the raycast
		 */
		Vec3 point = Vec3::zero;
		/*
		 * Hit normal of the raycast
		 */
		Vec3 normal = Vec3::zero;
		/*
		 * Collider hit by the raycast
		 */
		shared_ptr<Collider> collider;
	};

	struct RayParams
	{
		bool can_hit_from_origin = true;
		//  TODO: add collision layer
	};
}