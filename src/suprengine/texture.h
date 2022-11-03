#pragma once
#include "texture.fwd.h"
#include "render_batch.h"

namespace suprengine
{
	class Texture
	{
	private:
		std::string path { "" };
		Vec2 size { 0.0f, 0.0f };
		SDL_Texture* sdl_texture { nullptr };
	public:
		Texture( std::string path, SDL_Texture* sdl_texture, const Vec2& size )
			: path( path ), sdl_texture( sdl_texture ), size( size ) {}
		~Texture();

		static Texture* load( RenderBatch* render_batch, const std::string& path );
		static Texture* load_from_surface( RenderBatch* render_batch, const std::string& path, SDL_Surface* surface );
		static SDL_Surface* load_surface( const std::string& path );
		
		static uint32_t get_pixel_at( SDL_Surface* surface, int x, int y );
		static Color get_pixel_color_at( SDL_Surface* surface, int x, int y );

		std::string get_path() const { return path; }
		Vec2 get_size() const { return size; };
		SDL_Texture* get_sdl_texture() const { return sdl_texture; }
	};
}