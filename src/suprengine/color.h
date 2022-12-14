#pragma once

#include <SDL.h>

namespace suprengine
{
	struct Color
	{
		static const Color white, black, red, green, blue, transparent;

		uint8_t r = 255u, g = 255u, b = 255u, a = 255u;

		static Color from_pixel( uint32_t pixel )
		{
			return Color {
				(uint8_t) pixel,
				(uint8_t) ( pixel >> 8 ),
				(uint8_t) ( pixel >> 16 ),
				(uint8_t) ( pixel >> 24 )
			};
		}

		uint32_t to_pixel() const
		{
			uint32_t pixel = a << 24 | b << 16 | g << 8 | r;
			return pixel;
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