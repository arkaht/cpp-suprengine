#pragma once

#include <SDL.h>

#include "vec2.h"

#include "suprengine/utils/usings.h"

namespace suprengine
{
	enum class SplitDirection : uint8
	{
		Horizontal,
		Vertical,
	};

	struct Rect
	{
	public:
		float x = 0.0f;
		float y = 0.0f;
		float w = 0.0f;
		float h = 0.0f;

	public:
		constexpr Rect() = default;
		explicit constexpr Rect( const float x, const float y, const float w, const float h )
			: x( x ), y( y ), w( w ), h( h ) {};
		explicit constexpr Rect( const Vec2 pos, const Vec2 size )
			: x( pos.x ), y( pos.y ), w( size.x ), h( size.y ) {};

		Rect& operator+=( const Vec2& v )
		{
			x += v.x;
			y += v.y;
			return *this;
		}

		Rect operator*( const float m ) const
		{
			return Rect {
				x * m,
				y * m,
				w * m,
				h * m,
			};
		}

		Rect operator/( const float m ) const
		{
			return Rect {
				x / m,
				y / m,
				w / m,
				h / m,
			};
		}

		/**
		 * Split a rectangle either horizontally or vertically, with a given ratio.
		 * @param direction Direction to split the rectangle.
		 * @param ratio Percent of size to keep for this rectangle, and to remove from the other.
		 * @return The new rectangle that forms the remaining of what this rectangle has lost.
		 */
		Rect split( SplitDirection direction, float ratio = 0.5f );

		Vec2 get_pos() const;
		Vec2 get_size() const;

		void add_pos( const Vec2& pos );
		void set_pos( const Vec2& pos );
		void set_size( const Vec2& size );

		SDL_Rect to_sdl_rect() const;
		std::string to_string() const;
	};
}
