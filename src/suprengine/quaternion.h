#pragma once

#include "vec3.h"

namespace suprengine
{
	class Quaternion
	{
	public:
		float x;
		float y;
		float z;
		float w;

		Quaternion()
		{
			*this = Quaternion::identity;
		}
		// This directly sets the quaternion components --
		// don't use for axis/angle
		explicit Quaternion( float inX, float inY, float inZ, float inW );
		// Construct the quaternion from an axis and angle
		// It is assumed that axis is already normalized,
		// and the angle is in radians
		explicit Quaternion( const Vec3& axis, float angle );
		explicit Quaternion( const Vec3& angles );

		void set( float inX, float inY, float inZ, float inW );
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