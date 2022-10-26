#pragma once

#include <SDL.h>

#include "window.h"
#include "rect.h"
#include "color.h"
#include "ecs/components/renderer.fwd.h"

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

		bool initialize( Window* window );
		void close();

		void begin_render();
		void render();
		void end_render();

		void draw_rect( Rect& rect, Color& color );

		void add_renderer( Renderer* renderer );
		void remove_renderer( Renderer* renderer );
	};
}

