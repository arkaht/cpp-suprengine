#pragma once

#include "math.h"

#include <SDL.h>

namespace suprengine
{
	struct Color
	{
		static const Color white, black;
		static const Color red, green, blue;
		static const Color purple, orange, duckblue;
		static const Color gray;
		static const Color transparent;

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
		//  Big endian version of Color::from_pixel()
		static Color from_0x( uint32_t hex )
		{
			return Color {
				(uint8_t) ( hex >> 24 ),
				(uint8_t) ( hex >> 16 ),
				(uint8_t) ( hex >> 8 ),
				(uint8_t) hex
			};
		}
		static Color from_hex( std::string hex )
		{
			//  Remove optional hexadecimal indicator
			if ( hex[0] == '#' )
			{
				hex = hex.erase( 0, 1 );
			}

			return Color {
				math::from_hex( hex.substr( 0, 2 ) ),
				math::from_hex( hex.substr( 2, 2 ) ),
				math::from_hex( hex.substr( 4, 2 ) ),
				math::from_hex( hex.substr( 6, 2 ) ),
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
			return a << 24 | b << 16 | g << 8 | r;
		}
		uint32_t to_0x() const
		{
			return r << 24 | g << 16 | b << 8 | a;
		}
		std::string to_hex() const
		{
			return "#" 
				 + math::to_hex( r ) 
				 + math::to_hex( g ) 
				 + math::to_hex( b ) 
				 + math::to_hex( a );
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