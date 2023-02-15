#pragma once

#include <suprengine/texture.h>

namespace suprengine
{
	class OpenGLTexture : public Texture
	{
	private:
		GLuint texture_id { 0 };
	
	public:
		OpenGLTexture( rconst_str path, SDL_Surface* surface ) 
			: Texture( path, Vec2 { (float) surface->w, (float) surface->h } )
		{
			uint8_t bpp = surface->format->BytesPerPixel;

			//  get pixel format
			int format = 0;
			std::string format_str = "NONE";


			if ( bpp == 4 )
			{
				format = surface->format->Rmask == 0x000000FF ? GL_RGBA : GL_BGRA;
			}
			else
			{
				format = surface->format->Rmask == 0x000000FF ? GL_RGB : GL_BGR;
			}

			//  generate textures
			glGenTextures( 1, &texture_id );
			glBindTexture( GL_TEXTURE_2D, texture_id );
			glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0, format, GL_UNSIGNED_BYTE, surface->pixels );

			//  enable bilinear filtering
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );


			Logger::info( 
				"created texture '" + path + 
				"' (" + std::to_string( surface->format->format ) + 
				": " + format_str + ")"
			);
		}

		~OpenGLTexture()
		{
			glDeleteTextures( 1, &texture_id );
		}

		void activate() override
		{
			glBindTexture( GL_TEXTURE_2D, texture_id );
		}
	};
}