#pragma once

#include <SDL.h>

#include <suprengine/math.h>

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

		//  big endian version of Color::from_pixel
		static Color from_0x( uint32_t hex )
		{
			return Color {
				(uint8_t) ( hex >> 24 ),
				(uint8_t) ( hex >> 16 ),
				(uint8_t) ( hex >> 8 ),
				(uint8_t) hex
			};
		}

		static Color lerp( const Color& a, const Color& b, float t )
		{
			return Color {
				math::lerp( a.r, b.r, t ),
				math::lerp( a.g, b.g, t ),
				math::lerp( a.b, b.b, t ),
				math::lerp( a.a, b.a, t )
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