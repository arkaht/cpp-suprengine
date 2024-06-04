#pragma once

#include <string>

#include "math.h"
#include "vec2.h"

namespace suprengine
{
	class Vec3
	{
	public:
		static const Vec3 zero, one;
		static const Vec3 forward, right, up;
		static const Vec3 unit_x, unit_y, unit_z;
		static const Vec3 infinity;

	public:
		float x { 0.0f };
		float y { 0.0f };
		float z { 0.0f };

	public:
		explicit Vec3();
		explicit Vec3( float value );
		explicit Vec3( float x, float y, float z );

		explicit Vec3( const Vec2& vec );
		explicit Vec3( const Vec2& vec, float z );

	public:
		/*  
		 * Returns the vector squared magnitude within 3-dimensions.
		 */
		float length_sqr() const;
		/* 
		 * Returns the vector magnitude within 3-dimensions.
		 */
		float length() const;

		/*  
		 * Returns the vector squared magnitude within 2-dimensions
		 * (X and Y).
		 */
		float length2d_sqr() const;
		/*
		 * Returns the vector magnitude within 2-dimensions 
		 * (X and Y).
		 */
		float length2d() const;

		/*
		 * Normalizes the vector within 3-dimensions.
		 */
		void normalize();
		/*
		 * Normalizes the vector within 2-dimensions.
		 * Z-axis is set to 0.0f.
		 */
		void normalize2d();

		/*
		 * Returns a copy of the vector normalized within 3-dimensions.
		 */
		Vec3 normalized() const;
		
		/*
		 * Returns a string representing the vector.
		 */
		std::string to_string() const;

		const float* get_as_float_ptr() const;

	public:
		static float distance( const Vec3& from, const Vec3& to );
		static float distance_sqr( const Vec3& from, const Vec3& to );
		
		static float distance2d( const Vec3& from, const Vec3& to );
		static float distance2d_sqr( const Vec3& from, const Vec3& to );

		static Vec3 snap_to_grid( const Vec3& v, float grid_size );
		static Vec3 snap_to_grid( const Vec3& v, const Vec3& grid_size );

		// Normalize the provided vector
		static Vec3 normalize( const Vec3& vec );
		// Dot product between two vectors (a dot b)
		static float dot( const Vec3& a, const Vec3& b );
		// Cross product between two vectors (a cross b)
		static Vec3 cross( const Vec3& a, const Vec3& b );
		static Vec3 lerp( const Vec3& a, const Vec3& b, float t );
		// Reflect V about (normalized) N
		static Vec3 reflect( const Vec3& v, const Vec3& n );
		static Vec3 approach( const Vec3& current, const Vec3& target, float delta );
		static Vec3 sqrt( const Vec3& current );

		// Transform a Vector3 by a quaternion
		static Vec3 transform( const Vec3& v, const class Quaternion& q );
		static Vec3 transform( const Vec3& vec, const class Mtx4& mat, float w = 1.0f );
		// This will transform the vector and renormalize the w component
		static Vec3 transform_with_perspective_div( const Vec3& vec, const class Mtx4& mat, float w = 1.0f );

	public:
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

		Vec3 operator/( const Vec3& v )
		{
			return Vec3( x / v.x, y / v.y, z / v.z );
		}
		Vec3 operator/( float scalar )
		{
			return Vec3( x / scalar, y / scalar, z / scalar );
		}

		// Scalar *=
		Vec3& operator*=( float scalar )
		{
			x *= scalar;
			y *= scalar;
			z *= scalar;
			return *this;
		}
		Vec3& operator*=( const Vec3& v )
		{
			x *= v.x;
			y *= v.y;
			z *= v.z;
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

		Vec3 operator-() const { return Vec3 { -x, -y, -z }; }

		bool operator==( const Vec2& vec2 )
		{
			return math::near_value( x, vec2.x ) 
			    && math::near_value( y, vec2.y )
			    && math::near_value( z, 0.0f );
		}

		bool operator==( const Vec3& vec3 )
		{
			return math::near_value( x, vec3.x )
			    && math::near_value( y, vec3.y )
			    && math::near_value( z, vec3.z );
		}
	};
}
