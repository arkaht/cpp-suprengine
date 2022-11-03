#include "font.h"

using namespace suprengine;

Font* Font::load( const std::string& path, int size )
{
	TTF_Font* sdl_font = TTF_OpenFont( path.c_str(), size );
	if ( sdl_font == nullptr )
	{
		Logger::error( "failed to open font from file " + path );
		return nullptr;
	}

	return new Font( path, sdl_font, size );
}
