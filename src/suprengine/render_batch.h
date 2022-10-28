#pragma once

#include <SDL.h>
#include <SDL_image.h>

#include "window.h"
#include "rect.h"
#include "color.h"
#include "texture.fwd.h"
#include <suprengine/ecs/components/renderers/renderer.fwd.h>

#include <vector>

namespace suprengine
{
	class RenderBatch
	{
	private:
		SDL_Renderer* _sdl_renderer { nullptr };

		std::vector<Renderer*> renderers;
	public:
		RenderBatch() {};
		RenderBatch( const RenderBatch& ) = delete;
		RenderBatch& operator=( const RenderBatch& ) = delete;
		~RenderBatch();

		bool initialize( Window* window );

		void begin_render();
		void render();
		void end_render();

		void draw_rect( const Rect& rect, const Color& color );
		void draw_texture( const Rect& src_rect, const Rect& dest_rect, const double rotation, const Vec2& origin, Texture* texture, const Color& color );

		void add_renderer( Renderer* renderer );
		void remove_renderer( Renderer* renderer );

		SDL_Renderer* get_sdl_renderer() const { return _sdl_renderer; }
	};
}
