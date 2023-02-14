#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include <suprengine/render_batch.h>

namespace suprengine
{
	class SDLRenderBatch : public RenderBatch
	{
	private:
		SDL_Renderer* sdl_renderer { nullptr };
	public:
		SDLRenderBatch() : RenderBatch() {};
		SDLRenderBatch( const SDLRenderBatch& ) = delete;
		SDLRenderBatch& operator=( const SDLRenderBatch& ) = delete;
		~SDLRenderBatch();

		bool initialize( Window* window ) override;

		void begin_render() override;
		void end_render() override;

		void draw_rect( DrawType draw_type, const Rect& rect, const Color& color ) override;
		void draw_texture( const Rect& src_rect, const Rect& dest_rect, const double rotation, const Vec2& origin, Texture* texture, const Color& color ) override;

		void scale( float zoom ) override;
		void clip( const Rect& region ) override;

		SDL_Texture* load_texture_from_surface( SDL_Surface* surface ) override;

		SDL_Renderer* get_sdl_renderer() const { return sdl_renderer; }
	};
}

