#pragma once
#include <suprengine/render-batch.h>

#include "sdl-texture.hpp"

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

namespace suprengine
{
	class SDLRenderBatch : public RenderBatch
	{
	private:
		SDL_Renderer* sdl_renderer { nullptr };
	public:
		SDLRenderBatch( Window* window ) : RenderBatch( window ) {};
		SDLRenderBatch( const SDLRenderBatch& ) = delete;
		SDLRenderBatch& operator=( const SDLRenderBatch& ) = delete;
		~SDLRenderBatch();

		bool initialize() override;

		void begin_render() override;
		void end_render() override;

		void draw_rect( DrawType draw_type, const Rect& rect, const Color& color ) override;
		void draw_texture( const Rect& src_rect, const Rect& dest_rect, float rotation, const Vec2& origin, Texture* texture, const Color& color ) override;

		void scale( float zoom ) override;
		void clip( const Rect& region ) override;

		Texture* load_texture_from_surface( rconst_str path, SDL_Surface* surface, const TextureParams& params = {} ) override;

		SDL_Renderer* get_sdl_renderer() const { return sdl_renderer; }
	};
}

