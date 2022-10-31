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

		Vec2& approach( Vec2 target, float delta )
		{
			x = math::approach( x, target.x, delta );
			y = math::approach( y, target.y, delta );
			return *this;
		}

		SDL_Point to_sdl_point() const
		{
			return SDL_Point {
				static_cast<int>( x ),
				static_cast<int>( y )
			};
		}
	};
}