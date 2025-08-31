#pragma once

#include <SDL.h>
#include <string>

struct ImVec2;

namespace suprengine
{
	struct Vec3;

	enum class Axis2D
	{
		X = 0,
		Y = 1,
	};

	struct Vec2
	{
	public:
		static const Vec2 zero, one;
		static const Vec2 up, down, left, right;
		static const Vec2 unit_x, unit_y;
		static const Vec2 infinity;

	public:
		float x = 0.0f;
		float y = 0.0f;

	public:
		constexpr Vec2() = default;
		explicit constexpr Vec2( const float value ) : x( value ), y( value ) {}
		explicit constexpr Vec2( const float x, const float y ) : x( x ), y( y ) {}

		explicit Vec2( const ImVec2& vec );
		explicit Vec2( const Vec3& vec );

	public:
		/*
		 * Returns the vector magnitude.
		 */
		float length() const;
		/*
		 * Returns the vector squared magnitude.
		 */
		float length_sqr() const;

		/*
		 * Normalizes the vector.
		 */
		Vec2& normalize();
		/*
		 * Returns a copy of the vector normalized.
		 */
		Vec2 normalized() const;

		void set_axis( Axis2D axis, float value );

		float get_angle() const;

		SDL_Point to_sdl_point() const;

		/*
		 * Returns a string representing the vector.
		 */
		std::string to_string() const;

	public:
		static float distance( const Vec2& from, const Vec2& to );
		static float distance_sqr( const Vec2& from, const Vec2& to );

		static Vec2 direction( const Vec2& from, const Vec2& to );

		static Vec2 world_to_grid( const Vec2& value, float grid_size );

		static Vec2 snap_to_grid( const Vec2& value, float grid_size );
		static Vec2 snap_to_grid( const Vec2& value, const Vec2& grid_size );

		static Vec2 clamp( const Vec2& value, const Vec2& min, const Vec2& max );

		static Vec2 round( const Vec2& value );

		static float dot( const Vec2& a, const Vec2& b );

		static Vec2 lerp( const Vec2& a, const Vec2& b, float t );
		static Vec2 slerp( const Vec2& a, const Vec2& b, float t );
		static Vec2 nlerp( const Vec2& a, const Vec2& b, float t );

		static Vec2 approach( const Vec2& current, const Vec2& target, float delta );

	public:
		friend Vec2 operator+( const Vec2& lhs, const Vec2& rhs );
		friend Vec2 operator-( const Vec2& lhs, const Vec2& rhs );
		friend Vec2 operator*( const Vec2& lhs, const Vec2& rhs );
		friend Vec2 operator*( const Vec2& lhs, float rhs );
		friend Vec2 operator*( float lhs, const Vec2& rhs );
		friend Vec2 operator/( const Vec2& lhs, const Vec2& rhs );
		friend Vec2 operator/( const Vec2& lhs, float rhs );

		Vec2& operator+=( const Vec2& rhs );
		Vec2& operator-=( const Vec2& rhs );
		Vec2& operator*=( const Vec2& rhs );
		Vec2& operator*=( float rhs );
		Vec2& operator/=( const Vec2& rhs );
		Vec2& operator/=( float rhs );

		Vec2 operator-() const;

		friend bool operator==( const Vec2& lhs, const Vec2& rhs );
		friend bool operator!=( const Vec2& lhs, const Vec2& rhs );
	};
}
