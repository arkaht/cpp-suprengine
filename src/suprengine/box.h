#pragma once

#include "vec3.h"

namespace suprengine
{
	struct Box
	{
	public:
		static Box one;
		static Box half;

	public:
		Vec3 min = Vec3::zero;
		Vec3 max = Vec3::zero;

	public:
		Vec3 get_center() const;
		Vec3 get_size() const;

	public:
		Box operator+( const Vec3& pos ) const
		{
			return Box {
				pos + min,
				pos + max
			};
		}
		Box operator*( const Vec3& size ) const
		{
			return Box {
				min * size,
				max * size
			};
		}
		Box operator*( float value ) const
		{
			return Box {
				min * value,
				max * value
			};
		}
	};
}