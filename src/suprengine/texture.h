#pragma once
#include "texture.fwd.h"
#include "render_batch.h"

namespace suprengine
{
	class Texture
	{
	private:
		Texture( std::string filename, SDL_Texture* sdl_texture, const Vec2& size )
			: filename( filename ), sdl_texture( sdl_texture ), size( size ) {}

		std::string filename { "" };
		Vec2 size { 0.0f, 0.0f };
		SDL_Texture* sdl_texture { nullptr };
	public:
		~Texture();

		static Texture* load( RenderBatch* render_batch, const std::string& filename );
		static SDL_Surface* load_surface( const std::string& filename );

		std::string get_filename() const { return filename; }
		Vec2 get_size() const { return size; };
		SDL_Texture* get_sdl_texture() const { return sdl_texture; }
	};
}