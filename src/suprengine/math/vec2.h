#pragma once

#include <suprengine/math/math.h>

//	TODO: Implement IM_VEC2_CLASS_EXTRA
#include <imgui.h>
#include <SDL.h>

namespace suprengine
{
	enum class Axis2D
	{
		X = 0,
		Y = 1,
	};

	struct Vec2
	{
		static const Vec2 zero, one, up, down, left, right;

		float x { 0.0f }, y { 0.0f };

		constexpr Vec2() = default;
		constexpr explicit Vec2( const float value )
			: x( value ), y( value )
		{}
		constexpr Vec2( const float x, const float y )
			: x( x ), y( y )
		{}
		constexpr explicit Vec2( const ImVec2& vec )
			: x( vec.x ), y( vec.y )
		{}

		void set_axis( Axis2D axis, float value );

		Vec2 operator-() const
		{
			return Vec2 { -x, -y };
		}

		Vec2& operator+=( const Vec2& v )
		{
			x += v.x;
			y += v.y;
			return *this;
		}
		Vec2& operator*=( const Vec2& v )
		{
			x *= v.x;
			y *= v.y;
			return *this;
		}
		Vec2& operator*=( float v )
		{
			x *= v;
			y *= v;
			return *this;
		}
		Vec2& operator/=( const Vec2& v )
		{
			x /= v.x;
			y /= v.y;
			return *this;
		}
		Vec2& operator/=( float v )
		{
			x /= v;
			y /= v;
			return *this;
		}

		Vec2 operator+( const Vec2& v ) const
		{
			return Vec2 { x + v.x, y + v.y };
		}
		Vec2 operator-( const Vec2& v ) const
		{
			return Vec2 { x - v.x, y - v.y };
		}

		Vec2 operator*( const Vec2& v ) const
		{
			return Vec2 { x * v.x, y * v.y };
		}
		Vec2 operator*( float m ) const
		{
			return Vec2 { x * m, y * m };
		}

		Vec2 operator/( const Vec2& v ) const
		{
			return Vec2 { x / v.x, y / v.y };
		}
		Vec2 operator/( float m ) const
		{
			return Vec2 { x / m, y / m };
		}

		bool operator==( const Vec2& v ) const
		{
			return x == v.x && y == v.y;
		}

		Vec2& normalize()
		{
			float mag = length();
			if ( mag == 0.0f )
				return *this;

			x /= mag, y /= mag;
			return *this;
		}

		float length() const
		{
			return math::sqrt( length_sqr() );
		}

		float length_sqr() const
		{
			return x * x + y * y;
		}

		float get_angle() const
		{
			return (float)math::atan2( y, x );
		}

		static Vec2 lerp( const Vec2& a, const Vec2& b, float f )
		{
			return Vec2 {
				math::lerp( a.x, b.x, f ),
				math::lerp( a.y, b.y, f ),
			};
		}

		static float dot( const Vec2& a, const Vec2& b );

		static Vec2 approach( const Vec2& current, const Vec2& target, float delta )
		{
			return Vec2 {
				math::approach( current.x, target.x, delta ),
				math::approach( current.y, target.y, delta )
			};
		}

		SDL_Point to_sdl_point() const
		{
			return SDL_Point {
				(int)x,
				(int)y
			};
		}

		/*
		 * Returns a string representing the vector.
		 */
		std::string to_string() const;
	};
}