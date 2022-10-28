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
				static_cast<unsigned char>( pixel ),
				static_cast<unsigned char>( pixel >> 8 ),
				static_cast<unsigned char>( pixel >> 16 ),
				static_cast<unsigned char>( pixel >> 24 )
			};
		}

		SDL_Color to_sdl_color()
		{
			return SDL_Color { r, g, b, a };
		}
	};
}