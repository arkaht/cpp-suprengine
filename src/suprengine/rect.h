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

		SDL_Rect to_sdl_rect()
		{
			return SDL_Rect {
				static_cast<int>( x ),
				static_cast<int>( y ),
				static_cast<int>( w ),
				static_cast<int>( h ),
			};
		}

		Rect add_pos( const Vec2& v )
		{
			return Rect( x + v.x, y + v.y, w, h );
		}
	};
}