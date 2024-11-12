#pragma once

#include "vec3.h"

namespace suprengine
{
	struct Box
	{
		Vec3 min, max;

		Vec3 get_center() const { return ( min + max ) * 0.5f; }
		Vec3 get_size() const { return Vec3::zero; }  //  TODO: implement this
	
		Box operator+( const Vec3& pos ) const { return Box { pos + min, pos + max }; }
		Box operator*( const Vec3& size ) const { return Box { min * size, max * size }; }
		
		static Box ONE;
		static Box HALF;
	};
}