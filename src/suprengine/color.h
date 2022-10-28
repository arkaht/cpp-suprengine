#pragma once

#include <SDL.h>

namespace suprengine
{
	struct Color
	{
		static const Color white, red, green, blue, transparent;

		uint8_t r = 255u, g = 255u, b = 255u, a = 255u;

		static Color from_pixel( uint32_t pixel )
		{
			return Color {
				static_cast<uint8_t>( pixel ),
				static_cast<uint8_t>( pixel >> 8 ),
				static_cast<uint8_t>( pixel >> 16 ),
				static_cast<uint8_t>( pixel >> 24 )
			};
		}

		bool operator==( const Color& c ) const
		{
			return r == c.r && g == c.g && b == c.b && a == c.a;
		}

		SDL_Color to_sdl_color()
		{
			return SDL_Color { r, g, b, a };
		}
	};
}