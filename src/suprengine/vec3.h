#pragma once

#include "math.h"
#include "vec2.h"

namespace suprengine
{
	class Vec3
	{
	public:
		float x;
		float y;
		float z;

		Vec3() : x( 0.0f ), y( 0.0f ), z( 0.0f ) {}
		Vec3( const Vec2& vec2 ) : x( vec2.x ), y( vec2.y ), z( 0.0f ) {}

		explicit Vec3( float xP, float yP, float zP )
			:x( xP ), y( yP ), z( zP )
		{}

		void set( float xP, float yP, float zP );
		float length_sqr() const;
		float length() const;
		void normalize();

		Vec3 normalized() const;

		const float* getAsFloatPtr() const
		{
			return reinterpret_cast<const float*>( &x );
		}

		// Vector addition (a + b)
		friend Vec3 operator+( const Vec3& a, const Vec3& b )
		{
			return Vec3( a.x + b.x, a.y + b.y, a.z + b.z );
		}

		// Vector subtraction (a - b)
		friend Vec3 operator-( const Vec3& a, const Vec3& b )
		{
			return Vec3( a.x - b.x, a.y - b.y, a.z - b.z );
		}

		// Component-wise multiplication
		friend Vec3 operator*( const Vec3& left, const Vec3& right )
		{
			return Vec3( left.x * right.x, left.y * right.y, left.z * right.z );
		}

		// Scalar multiplication
		friend Vec3 operator*( const Vec3& vec, float scalar )
		{
			return Vec3( vec.x * scalar, vec.y * scalar, vec.z * scalar );
		}

		// Scalar multiplication
		friend Vec3 operator*( float scalar, const Vec3& vec )
		{
			return Vec3( vec.x * scalar, vec.y * scalar, vec.z * scalar );
		}

		// Scalar *=
		Vec3& operator*=( float scalar )
		{
			x *= scalar;
			y *= scalar;
			z *= scalar;
			return *this;
		}

		// Vector +=
		Vec3& operator+=( const Vec3& right )
		{
			x += right.x;
			y += right.y;
			z += right.z;
			return *this;
		}

		// Vector -=
		Vec3& operator-=( const Vec3& right )
		{
			x -= right.x;
			y -= right.y;
			z -= right.z;
			return *this;
		}

		operator Vec2() { return Vec2 { x, y }; }

		bool operator==( const Vec2& vec2 )
		{
			return x == vec2.x
				&& y == vec2.y
				&& z == 0.0f;
		}

		bool operator==( const Vec3& vec3 )
		{
			return x == vec3.x
				&& y == vec3.y
				&& z == vec3.z;
		}

		// Normalize the provided vector
		static Vec3 normalize( const Vec3& vec )
		{
			Vec3 temp = vec;
			temp.normalize();
			return temp;
		}

		// Dot product between two vectors (a dot b)
		static float dot( const Vec3& a, const Vec3& b )
		{
			return ( a.x * b.x + a.y * b.y + a.z * b.z );
		}

		// Cross product between two vectors (a cross b)
		static Vec3 cross( const Vec3& a, const Vec3& b )
		{
			Vec3 temp;
			temp.x = a.y * b.z - a.z * b.y;
			temp.y = a.z * b.x - a.x * b.z;
			temp.z = a.x * b.y - a.y * b.x;
			return temp;
		}

		// Lerp from A to B by f
		static Vec3 lerp( const Vec3& a, const Vec3& b, float f )
		{
			return Vec3( a + f * ( b - a ) );
		}

		// Reflect V about (normalized) N
		static Vec3 reflect( const Vec3& v, const Vec3& n )
		{
			return v - 2.0f * Vec3::dot( v, n ) * n;
		}

		static Vec3 approach( const Vec3& current, const Vec3& target, float delta )
		{
			return Vec3 {
				math::approach( current.x, target.x, delta ),
				math::approach( current.y, target.y, delta ),
				math::approach( current.y, target.y, delta )
			};
		}

		static Vec3 transform( const Vec3& vec, const class Mtx4& mat, float w = 1.0f );

		// This will transform the vector and renormalize the w component
		static Vec3 transform_with_perspective_div( const Vec3& vec, const class Mtx4& mat, float w = 1.0f );

		// Transform a Vector3 by a quaternion
		static Vec3 transform( const Vec3& v, const class Quaternion& q );

		static const Vec3 zero, one;
		static const Vec3 right, up, forward;
		static const Vec3 unit_x, unit_y, unit_z;
		static const Vec3 neg_unit_x, neg_unit_y, neg_unit_z;
		static const Vec3 infinity, neg_infinity;
	};
}
