#pragma once

#include <suprengine/rendering/texture.h>

namespace suprengine
{
	class SDLTexture : public Texture
	{
	public:
		SDLTexture( SDL_Renderer* renderer, rconst_str path, SDL_Surface* surface )
			: Texture( path, Vec2 { static_cast<float>( surface->w ), static_cast<float>( surface->h ) } )
		{
			sdl_texture = SDL_CreateTextureFromSurface( renderer, surface );
		}

		~SDLTexture()
		{
			SDL_DestroyTexture( sdl_texture );
		}

		SDL_Texture* get_sdl_texture() const override { return sdl_texture; }

	private:
		SDL_Texture* sdl_texture { nullptr };
	};
}