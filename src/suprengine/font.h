#pragma once
#include "font.fwd.h"

#include <suprengine/sdl/sdl-render-batch.h>

//  TODO: abstract it
namespace suprengine
{
	class Font
	{
	private:
		std::string path { "" };
		int size { 0 };
		TTF_Font* sdl_font { nullptr };
	public:
		Font( std::string path, TTF_Font* sdl_font, int size )
			: path( path ), sdl_font( sdl_font ), size( size ) {}

		static Font* load( const std::string& path, int size );

		std::string get_path() const { return path; }
		int get_size() const { return size; };
		TTF_Font* get_sdl_font() const { return sdl_font; }
	};
}