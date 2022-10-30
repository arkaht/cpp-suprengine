#pragma once

#include <SDL.h>

#include "vec2.h"

namespace suprengine
{
	struct Rect
	{
		float x, y;
		float w, h;

		Rect( float x, float y, float w, float h ) 
			: x ( x ), y ( y ), w ( w ), h ( h ) {};
		Rect( Vec2 pos, Vec2 size ) 
			: x ( pos.x ), y ( pos.y ), w ( size.x ), h ( size.y ) {};

		Rect& operator+=( const Vec2& v )
		{
			x += v.x;
			y += v.y;
			return *this;
		}

		Rect& operator/( const float m )
		{
			x /= m;
			y /= m;
			w /= m;
			h /= m;
			return *this;
		}

		Vec2 get_pos() const
		{
			return Vec2 { x, y };
		}

		Vec2 get_size() const
		{
			return Vec2 { w, h };
		}

		SDL_Rect to_sdl_rect() const
		{
			return SDL_Rect {
				static_cast<int>( x ),
				static_cast<int>( y ),
				static_cast<int>( w ),
				static_cast<int>( h ),
			};
		}

		void add_pos( const Vec2& v )
		{
			x += v.x, y += v.y;
		}
	};
}