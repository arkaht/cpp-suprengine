#pragma once

#include <suprengine/vec3.h>
#include <suprengine/ecs/components/collider.fwd.h>

namespace suprengine
{
	struct Ray
	{
		Vec3 origin;
		Vec3 direction;
		float distance;

		Ray( Vec3 origin, Vec3 direction, float distance )
			: origin( origin ), direction( direction ), distance( distance ) {}
	};

	struct RayHit
	{
		Vec3 point;
		Vec3 normal;
		Collider* collider;
	};
}