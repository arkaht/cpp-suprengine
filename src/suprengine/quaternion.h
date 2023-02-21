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

		float get_x_angle()
		{
			return math::atan2( 2.0f * ( w * x + y * z ), 1.0f - 2.0f * ( x * x + y * y ) );
		}
		float get_y_angle()
		{
			return 2.0f * math::atan2(
				math::sqrt( 1.0f + 2.0f * ( w * y - x * z ) ),
				math::sqrt( 1.0f - 2.0f * ( w * y - x * z ) )
			) - math::HALF_PI;
		}
		float get_z_angle()
		{
			return math::atan2( 2.0f * ( w * z + x * y ), 1.0f - 2.0f * ( y * y + z * z ) );
		}
		Vec3 to_euler_angles()
		{
			return Vec3 {
				get_x_angle(),
				get_y_angle(),
				get_z_angle()
			};
		}

		float length_sqr() const
		{
			return ( x * x + y * y + z * z + w * w );
		}

		float length() const
		{
			return math::sqrt( length_sqr() );
		}

		// Normalize the provided quaternion
		static Quaternion normalize( const Quaternion& q )
		{
			Quaternion retVal = q;
			retVal.normalize();
			return retVal;
		}

		// Linear interpolation
		static Quaternion lerp( const Quaternion& a, const Quaternion& b, float f )
		{
			Quaternion retVal;
			retVal.x = math::lerp( a.x, b.x, f );
			retVal.y = math::lerp( a.y, b.y, f );
			retVal.z = math::lerp( a.z, b.z, f );
			retVal.w = math::lerp( a.w, b.w, f );
			retVal.normalize();
			return retVal;
		}

		static float dot( const Quaternion& a, const Quaternion& b )
		{
			return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
		}

		// Spherical Linear Interpolation
		static Quaternion slerp( const Quaternion& a, const Quaternion& b, float f )
		{
			float rawCosm = Quaternion::dot( a, b );

			float cosom = -rawCosm;
			if ( rawCosm >= 0.0f )
			{
				cosom = rawCosm;
			}

			float scale0, scale1;

			if ( cosom < 0.9999f )
			{
				const float omega = math::acos( cosom );
				const float invSin = 1.f / math::sin( omega );
				scale0 = math::sin( ( 1.f - f ) * omega ) * invSin;
				scale1 = math::sin( f * omega ) * invSin;
			}
			else
			{
				// Use linear interpolation if the quaternions
				// are collinear
				scale0 = 1.0f - f;
				scale1 = f;
			}

			if ( rawCosm < 0.0f )
			{
				scale1 = -scale1;
			}

			Quaternion retVal;
			retVal.x = scale0 * a.x + scale1 * b.x;
			retVal.y = scale0 * a.y + scale1 * b.y;
			retVal.z = scale0 * a.z + scale1 * b.z;
			retVal.w = scale0 * a.w + scale1 * b.w;
			retVal.normalize();
			return retVal;
		}

		// Concatenate
		// Rotate by q FOLLOWED BY p
		static Quaternion concatenate( const Quaternion& q, const Quaternion& p )
		{
			Quaternion retVal;

			// Vector component is:
			// ps * qv + qs * pv + pv x qv
			Vec3 qv( q.x, q.y, q.z );
			Vec3 pv( p.x, p.y, p.z );
			Vec3 newVec = p.w * qv + q.w * pv + Vec3::cross( pv, qv );
			retVal.x = newVec.x;
			retVal.y = newVec.y;
			retVal.z = newVec.z;

			// Scalar component is:
			// ps * qs - pv . qv
			retVal.w = p.w * q.w - Vec3::dot( pv, qv );

			return retVal;
		}

		static Quaternion look_at( const Vec3& origin, const Vec3& target, const Vec3& forward, const Vec3& up );

		static const Quaternion identity;
	};
}