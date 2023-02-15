#pragma once
#include "texture.fwd.h"

#include <suprengine/render-batch.h>

namespace suprengine
{
	class Texture
	{
	protected:
		std::string path { "" };
		Vec2 size { 0.0f, 0.0f };
	public:
		Texture( std::string path, const Vec2& size )
			: path( path ), size( size ) {}
		virtual ~Texture() {};

		//static Texture* load_from_surface( RenderBatch* render_batch, const std::string& path, SDL_Surface* surface, bool should_free_surface = true );
		static SDL_Surface* load_surface( rconst_str path );
		
		static uint32_t get_pixel_at( SDL_Surface* surface, int x, int y );
		static Color get_pixel_color_at( SDL_Surface* surface, int x, int y );

		std::string get_path() const { return path; }
		Vec2 get_size() const { return size; };

		virtual void activate() {}
		virtual SDL_Texture* get_sdl_texture() const { return nullptr; }
	};
}