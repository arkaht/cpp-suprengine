#include "texture.h"

using namespace suprengine;

Texture::~Texture()
{
	if ( sdl_texture == nullptr ) return;

	SDL_DestroyTexture( sdl_texture );
}

Texture* Texture::load( RenderBatch* render_batch, const std::string& filename )
{
	//  load surface
	SDL_Surface* surface = load_surface( filename );
	if ( surface == nullptr ) return nullptr;

	//  get width & height
	Vec2 size { surface->w, surface->h };

	//  load texture
	SDL_Texture* texture = SDL_CreateTextureFromSurface( render_batch->get_sdl_renderer(), surface );
	SDL_FreeSurface( surface );
	if ( texture == nullptr )
	{
		Logger::error( "failed to create texture from surface from file " + filename );
		return nullptr;
	}

	return new Texture( filename, texture, size );
}

SDL_Surface* Texture::load_surface( const std::string& filename )
{
	SDL_Surface* surface = IMG_Load( filename.c_str() );
	if ( surface == nullptr )
	{
		Logger::error( "failed to load surface from file " + filename );
		return nullptr;
	};

	return surface;
}

