#pragma once

#include "vec3.h"
#include "angles.h"

namespace suprengine
{
	class Quaternion
	{
	public:
		float x;
		float y;
		float z;
		float w;

	public:
		Quaternion()
		{
			*this = Quaternion::identity;
		}
		explicit Quaternion( float x, float y, float z, float w );
		explicit Quaternion( const Vec3& axis, float angle );

		explicit Quaternion( const RadAngles& angles );
		explicit Quaternion( const DegAngles& angles );

	public:
		void conjugate();
		void normalize();

		float get_x_angle();
		float get_y_angle();
		float get_z_angle();
		Vec3 to_euler_angles();

		float length() const;
		float length_sqr() const;

		// Normalize the provided quaternion
		static Quaternion normalize( const Quaternion& q );

		// Linear interpolation
		static Quaternion lerp( const Quaternion& a, const Quaternion& b, float f );
		// Spherical Linear Interpolation
		static Quaternion slerp( const Quaternion& a, const Quaternion& b, float f );

		static float dot( const Quaternion& a, const Quaternion& b );

		// Concatenate
		// Rotate by q FOLLOWED BY p
		static Quaternion concatenate( const Quaternion& q, const Quaternion& p );
		Quaternion operator+( const Quaternion& p ) { return concatenate( *this, p ); }

		static Quaternion look_at( const Vec3& forward, const Vec3& up );
		static Quaternion look_at( const Vec3& origin, const Vec3& target, const Vec3& up );

		static const Quaternion identity;
	};
}