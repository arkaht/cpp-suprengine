#pragma once

#include <suprengine/utils/memory.h>

#include <SDL_ttf.h>

#include <string>

namespace suprengine
{
	class Font
	{
	public:
		Font( std::string path, TTF_Font* sdl_font, int size )
			: path( path ), sdl_font( sdl_font ), size( size ) {}

		std::string get_path() const { return path; }
		int get_size() const { return size; };
		TTF_Font* get_sdl_font() const { return sdl_font; }

	public:
		static SharedPtr<Font> load( const std::string& path, int size );

	private:
		std::string path {};
		TTF_Font* sdl_font = nullptr;
		int size = 0;
	};
}