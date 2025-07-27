#pragma once

#include <suprengine/math/math.h>

//	TODO: Implement IM_VEC2_CLASS_EXTRA
#include <imgui.h>
#include <SDL.h>

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
		static const Vec2 zero;
		static const Vec2 one;
		static const Vec2 up;
		static const Vec2 down;
		static const Vec2 left;
		static const Vec2 right;

	public:
		float x = 0.0f;
		float y = 0.0f;

	public:
		constexpr Vec2() {};
		explicit constexpr Vec2( float value );
		explicit constexpr Vec2( float x, float y );
		explicit constexpr Vec2( const ImVec2& vec );
		explicit constexpr Vec2( const Vec3& vec );

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

		Vec2& operator*=( const float v )
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

		Vec2 operator*( const float m ) const
		{
			return Vec2 { x * m, y * m };
		}

		Vec2 operator/( const Vec2& v ) const
		{
			return Vec2 { x / v.x, y / v.y };
		}

		Vec2 operator/( const float m ) const
		{
			return Vec2 { x / m, y / m };
		}

		bool operator==( const Vec2& v ) const
		{
			return x == v.x && y == v.y;
		}

		Vec2& normalize()
		{
			const float mag = length();
			if ( mag == 0.0f ) return *this;

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
			return static_cast<float>( math::atan2( y, x ) );
		}

		static Vec2 lerp( const Vec2& a, const Vec2& b, const float t )
		{
			return Vec2 {
				math::lerp( a.x, b.x, t ),
				math::lerp( a.y, b.y, t ),
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
				static_cast<int>( x ),
				static_cast<int>( y )
			};
		}

		/*
		 * Returns a string representing the vector.
		 */
		std::string to_string() const;
	};
}
