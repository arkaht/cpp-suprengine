#pragma once

#include <SDL.h>
#include "math.h"

namespace suprengine
{
	struct Vec2
	{
		static const Vec2 zero, one, up, down, left, right;

		float x { 0.0f }, y { 0.0f };

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
			x /= mag, y /= mag;
			return *this;
		}

		float length() const
		{
			return sqrtf( length_sqr() );
		}

		float length_sqr() const
		{
			return x * x + y * y;
		}

		float get_angle() const
		{
			return (float) ( atan2( y, x ) * math::RAD2DEG );
		}

		Vec2& approach( Vec2 target, float delta )
		{
			x = math::approach( x, target.x, delta );
			y = math::approach( y, target.y, delta );
			return *this;
		}

		SDL_Point to_sdl_point() const
		{
			return SDL_Point {
				(int) x,
				(int) y
			};
		}
	};
}