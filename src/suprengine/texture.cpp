#include "texture.h"

using namespace suprengine;

Texture::~Texture()
{
	if ( sdl_texture == nullptr ) return;

	SDL_DestroyTexture( sdl_texture );
}

Texture* Texture::load( RenderBatch* render_batch, const std::string& path )
{
	//  load surface
	SDL_Surface* surface = load_surface( path );
	if ( surface == nullptr ) return nullptr;

	return load_from_surface( render_batch, path, surface );
}

Texture* Texture::load_from_surface( RenderBatch* render_batch, const std::string& path, SDL_Surface* surface )
{
	//  get width & height
	Vec2 size { (float) surface->w, (float) surface->h };

	//  load texture
	SDL_Texture* texture = SDL_CreateTextureFromSurface( render_batch->get_sdl_renderer(), surface );
	SDL_FreeSurface( surface );
	if ( texture == nullptr )
	{
		Logger::error( "failed to create texture from surface from file " + path );
		return nullptr;
	}

	return new Texture( path, texture, size );
}

SDL_Surface* Texture::load_surface( const std::string& path )
{
	SDL_Surface* surface = IMG_Load( path.c_str() );
	if ( surface == nullptr )
	{
		Logger::error( "failed to load surface from file " + path );
		return nullptr;
	};

	return surface;
}

//  https://stackoverflow.com/questions/53033971/how-to-get-the-color-of-a-specific-pixel-from-sdl-surface
uint32_t Texture::get_pixel_at( SDL_Surface* surface, int x, int y )
{
	int bpp = surface->format->BytesPerPixel;
	uint8_t* pixel = (uint8_t*) surface->pixels + y * surface->pitch + x * bpp;

	switch ( bpp )
	{
	case 1:
		return *pixel;
	case 2:
		return *(uint16_t*) pixel;
	case 3:
		if ( SDL_BYTEORDER == SDL_BIG_ENDIAN )
		{
			return pixel[0] << 16 | pixel[1] << 8 | pixel[2];
		}

		return pixel[0] | pixel[1] << 8 | pixel[2] << 16;
	case 4:
		return *(uint32_t*) pixel;
	}

	return 0;
}

Color Texture::get_pixel_color_at( SDL_Surface* surface, int x, int y )
{
	uint32_t pixel = Texture::get_pixel_at( surface, x, y );
	return Color::from_pixel( pixel );
}

