#pragma once
#include <suprengine/render-batch.h>

#include "sdl-texture.hpp"

struct SDL_Renderer;
struct SDL_Surface;

namespace suprengine
{
	class SDLRenderBatch : public RenderBatch
	{
	public:
		SDLRenderBatch( Window* window );
		SDLRenderBatch( const SDLRenderBatch& ) = delete;
		SDLRenderBatch& operator=( const SDLRenderBatch& ) = delete;
		~SDLRenderBatch();

		void begin_render() override;
		void end_render() override;

		void draw_rect( DrawType draw_type, const Rect& rect, const Color& color ) override;
		void draw_texture(
			const Rect& src_rect,
			const Rect& dest_rect,
			float rotation,
			const Vec2& origin,
			SharedPtr<Texture> texture,
			const Color& color
		) override;

		void translate( const Vec2& pos ) override;
		void scale( float zoom ) override;
		void clip( const Rect& region ) override;

		SharedPtr<Texture> load_texture_from_surface(
			rconst_str path,
			SDL_Surface* surface,
			const TextureParams& params = {}
		) override;

		SDL_Renderer* get_sdl_renderer() const { return _sdl_renderer; }

	private:
		SDL_Renderer* _sdl_renderer { nullptr };

		Vec2 _translation { Vec2::zero };
	};
}

