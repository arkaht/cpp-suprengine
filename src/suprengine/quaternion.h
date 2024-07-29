#pragma once

#include "vec3.h"
#include "angles.h"

namespace suprengine
{
	class Quaternion
	{
	public:
		static const Quaternion identity;

	public:
		float x { 0.0f };
		float y { 0.0f };
		float z { 0.0f };
		float w { 1.0f };

	public:
		Quaternion() {}
		explicit Quaternion( float x, float y, float z, float w );
		explicit Quaternion( const Vec3& axis, float angle );

		explicit Quaternion( const RadAngles& angles );
		explicit Quaternion( const DegAngles& angles );

	public:
		void conjugate();
		void normalize();

		float get_radian_pitch() const;
		float get_radian_yaw() const;
		float get_radian_roll() const;

		RadAngles to_radians() const;
		DegAngles to_degrees() const;

		float length() const;
		float length_sqr() const;
		
	public:
		/*
		 * Returns a normalized quaternion.
		 */
		static Quaternion normalize( const Quaternion& quat );

		/*
		 * Returns the linear interpolation of two quaternions.
		 */
		static Quaternion lerp( 
			const Quaternion& a,
			const Quaternion& b,
			float t
		);
		/*
		 * Returns the spherical linear interpolation of two 
		 * quaternions.
		 */
		static Quaternion slerp( 
			const Quaternion& a,
			const Quaternion& b,
			float t
		);

		/*
		 * Returns the dot product of two quaternions.
		 */
		static float dot( 
			const Quaternion& a,
			const Quaternion& b
		);

		/*
		 * Returns the concatenated quaternion of 'q' followed by 
		 * 'p'.
		 */
		static Quaternion concatenate( 
			const Quaternion& q,
			const Quaternion& p
		);

		/*
		 * Returns a quaternion looking towards 'forward' and using 
		 * 'up'.
		 */
		static Quaternion look_at( 
			const Vec3& forward,
			const Vec3& up
		);
		/*
		 * Returns a quaternion looking from 'origin' to 'target'
		 * using 'up'.
		 */
		static Quaternion look_at( 
			const Vec3& origin,
			const Vec3& target,
			const Vec3& up
		);

	public:
		Quaternion operator+( const Quaternion& p )
		{
			return concatenate( *this, p );
		}
	};
}