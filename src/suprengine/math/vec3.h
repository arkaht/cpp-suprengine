#pragma once

#include <string>

#include "math.h"
#include "vec2.h"

namespace suprengine
{
	struct Quaternion;
	struct Mtx4;
	struct Vec4;

	struct Vec3
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
		Vec3() {}
		explicit Vec3( float value );
		explicit Vec3( float x, float y, float z );
		explicit Vec3( const Vec2& vec, float z = 0.0f );
		explicit Vec3( const Vec4& vec );

		Vec3& operator=( float value );

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
		 * Returns a copy of the vector normalized within 2-dimensions.
		 */
		Vec3 normalized2d() const;
		
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

		static Vec3 direction( const Vec3& from, const Vec3& to );
		static Vec3 direction2d( const Vec3& from, const Vec3& to );

		static Vec3 world_to_grid( const Vec3& value, float grid_size );

		static Vec3 snap_to_grid( const Vec3& value, float grid_size );
		static Vec3 snap_to_grid( const Vec3& value, const Vec3& grid_size );

		static Vec3 clamp( const Vec3& value, const Vec3& min, const Vec3& max );

		static Vec3 round( const Vec3& value );

		//  Dot product between two vectors (a dot b)
		static float dot( const Vec3& a, const Vec3& b );
		//  Cross product between two vectors (a cross b)
		static Vec3 cross( const Vec3& a, const Vec3& b );

		static Vec3 lerp( const Vec3& a, const Vec3& b, float t );
		static Vec3 slerp( const Vec3& a, const Vec3& b, float t );
		static Vec3 nlerp( const Vec3& a, const Vec3& b, float t );

		//  Reflect V about (normalized) N
		static Vec3 reflect( const Vec3& value, const Vec3& n );
		static Vec3 approach( const Vec3& current, const Vec3& target, float delta );
		static Vec3 sqrt( const Vec3& current );

		//  Transform a Vector3 by a quaternion
		static Vec3 transform( const Vec3& value, const Quaternion& q );
		static Vec3 transform( const Vec3& vec, const Mtx4& mat, float w = 1.0f );
		//  This will transform the vector and renormalize the w component
		static Vec3 transform_with_perspective_div( const Vec3& vec, const Mtx4& mat, float w = 1.0f );

	public:
		//  Vector addition (a + b)
		friend Vec3 operator+( const Vec3& a, const Vec3& b )
		{
			return Vec3( a.x + b.x, a.y + b.y, a.z + b.z );
		}

		//  Vector subtraction (a - b)
		friend Vec3 operator-( const Vec3& a, const Vec3& b )
		{
			return Vec3( a.x - b.x, a.y - b.y, a.z - b.z );
		}

		//  Component-wise multiplication
		friend Vec3 operator*( const Vec3& left, const Vec3& right )
		{
			return Vec3( left.x * right.x, left.y * right.y, left.z * right.z );
		}

		//  Scalar multiplication
		friend Vec3 operator*( const Vec3& vec, float scalar )
		{
			return Vec3( vec.x * scalar, vec.y * scalar, vec.z * scalar );
		}

		//  Scalar multiplication
		friend Vec3 operator*( float scalar, const Vec3& vec )
		{
			return Vec3( vec.x * scalar, vec.y * scalar, vec.z * scalar );
		}

		Vec3 operator/( const Vec3& value )
		{
			return Vec3( x / value.x, y / value.y, z / value.z );
		}
		Vec3 operator/( float scalar )
		{
			return Vec3( x / scalar, y / scalar, z / scalar );
		}

		Vec3& operator*=( float scalar )
		{
			x *= scalar;
			y *= scalar;
			z *= scalar;
			return *this;
		}
		Vec3& operator*=( const Vec3& value )
		{
			x *= value.x;
			y *= value.y;
			z *= value.z;
			return *this;
		}

		Vec3& operator+=( const Vec3& right )
		{
			x += right.x;
			y += right.y;
			z += right.z;
			return *this;
		}

		Vec3& operator-=( const Vec3& right )
		{
			x -= right.x;
			y -= right.y;
			z -= right.z;
			return *this;
		}

		operator Vec2() { return Vec2 { x, y }; }

		Vec3 operator-() const { return Vec3 { -x, -y, -z }; }

		friend bool operator==( const Vec3& a, const Vec2& b )
		{
			return math::near_value( a.x, b.x )
			    && math::near_value( a.y, b.y )
			    && math::near_value( a.z, 0.0f );
		}

		friend bool operator==( const Vec3& a, const Vec3& b )
		{
			return math::near_value( a.x, b.x )
			    && math::near_value( a.y, b.y )
			    && math::near_value( a.z, b.z );
		}
	};
}
