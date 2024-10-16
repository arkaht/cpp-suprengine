#pragma once

#include <suprengine/render-batch.h>

namespace suprengine
{
	class Texture
	{
	public:
		Texture( rconst_str path, Vec2 size )
			: path( path ), size( size ) {}
		Texture( rconst_str path, SDL_Surface* surface, const TextureParams& params );
		~Texture();

		//static Texture* load_from_surface( RenderBatch* render_batch, const std::string& path, SDL_Surface* surface, bool should_free_surface = true );
		static SDL_Surface* load_surface( rconst_str path );
		
		static uint32_t get_pixel_at( SDL_Surface* surface, int x, int y );
		static Color get_pixel_color_at( SDL_Surface* surface, int x, int y );

		std::string get_path() const { return path; }
		Vec2 get_size() const { return size; };

		void activate();

		virtual SDL_Texture* get_sdl_texture() const { return nullptr; }

	private:
		uint32 texture_id = 0;
		std::string path {};
		Vec2 size = Vec2::zero;
	};
}