#include "font.h"

#include <suprengine/utils/logger.h>

#include <SDL_ttf.h>

using namespace suprengine;

SharedPtr<Font> Font::load( const std::string& path, int size )
{
	TTF_Font* sdl_font = TTF_OpenFont( path.c_str(), size );
	if ( sdl_font == nullptr )
	{
		Logger::error( "Failed to open font from file " + path );
		return nullptr;
	}

	//	TODO: Doesn't it need TTF_CloseFont here as for SDL_FreeSurface?

	return std::make_shared<Font>( path, sdl_font, size );
}
