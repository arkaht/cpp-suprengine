#pragma once

#include <suprengine/texture.h>

namespace suprengine
{
	class SDLTexture : public Texture
	{
	private:
		SDL_Texture* sdl_texture { nullptr };

	public:
		SDLTexture( SDL_Renderer* renderer, rconst_str path, SDL_Surface* surface )
			: Texture( path, Vec2 { (float) surface->w, (float) surface->h } )
		{
			sdl_texture = SDL_CreateTextureFromSurface( renderer, surface );
		}

		~SDLTexture()
		{
			SDL_DestroyTexture( sdl_texture );
		}

		SDL_Texture* get_sdl_texture() const override { return sdl_texture; }
	};
}