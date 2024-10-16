#include "texture.h"

#include <suprengine/sdl/sdl-render-batch.h>

#include <suprengine/logger.h>

//#include <SDL.h>
#include <GL/glew.h>
#include <SDL_image.h>

using namespace suprengine;

//Texture* Texture::load_from_surface( RenderBatch* render_batch, const std::string& path, SDL_Surface* surface, bool should_free_surface )
//{
//	//  get width & height
//	Vec2 size { 
//		(float) surface->w, 
//		(float) surface->h 
//	};
//
//	//  load texture
//	//  TODO: abstract it
//	SDL_Texture* texture = render_batch->load_texture_from_surface( surface );
//
//	//  free surface
//	if ( should_free_surface )
//		SDL_FreeSurface( surface );
//
//	if ( texture == nullptr )
//	{
//		Logger::error( "failed to create texture from surface from file " + path );
//		return nullptr;
//	}
//
//	return new Texture( path, texture, size );
//}

Texture::Texture( rconst_str path, SDL_Surface* surface, const TextureParams& params )
	: path( path ), size { (float)surface->w, (float)surface->h }
{
	//  Get pixel format
	int format = 0;
	std::string format_str {};
	if ( surface->format->BytesPerPixel == 4 )
	{
		if ( surface->format->Rmask == 0x000000FF )
		{
			format = GL_RGBA;
			format_str = "GL_RGBA";
		}
		else
		{
			format = GL_BGRA;
			format_str = "GL_BGRA";
		}
	}
	else
	{
		if ( surface->format->Rmask == 0x000000FF )
		{
			format = GL_RGB;
			format_str = "GL_RGB";
		}
		else
		{
			format = GL_BGR;
			format_str = "GL_BGR";
		}
	}

	//  Generate textures
	glGenTextures( 1, &texture_id );
	glBindTexture( GL_TEXTURE_2D, texture_id );
	glTexImage2D(
		GL_TEXTURE_2D,
		/* level */ 0,
		GL_RGBA,
		surface->w, surface->h,
		/* boder */ 0,
		format, GL_UNSIGNED_BYTE,
		surface->pixels
	);

	//  Apply filtering
	int filter = params.filtering == FilteringType::Bilinear ? GL_LINEAR : GL_NEAREST;
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter );

	Logger::info(
		"Created texture from file '%s' (ID: %d; FORMAT: %s)",
		*path, texture_id, *format_str
	);
}

Texture::~Texture()
{
	glDeleteTextures( 1, &texture_id );
}

SDL_Surface* Texture::load_surface( rconst_str path )
{
	SDL_Surface* surface = IMG_Load( path.c_str() );
	if ( surface == nullptr )
	{
		Logger::error( "Failed to load surface from file '%s'", path );
		return nullptr;
	};

	return surface;
}

//  https://stackoverflow.com/questions/53033971/how-to-get-the-color-of-a-specific-pixel-from-sdl-surface
uint32_t Texture::get_pixel_at( SDL_Surface* surface, int x, int y )
{
	const int bpp = surface->format->BytesPerPixel;
	uint8_t* pixel = (uint8_t*)surface->pixels + y * surface->pitch + x * bpp;

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
	const uint32_t pixel = Texture::get_pixel_at( surface, x, y );
	return Color::from_pixel( pixel );
}

void Texture::activate()
{
	glBindTexture( GL_TEXTURE_2D, texture_id );
}

