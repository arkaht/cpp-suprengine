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

		void set( float inX, float inY, float inZ, float inW );
		void conjugate();
		void normalize();

		float lengthSq() const
		{
			return ( x * x + y * y + z * z + w * w );
		}

		float length() const
		{
			return math::sqrt( lengthSq() );
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

		static const Quaternion identity;
	};
}