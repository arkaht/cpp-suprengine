#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include "window.h"
#include "rect.h"
#include "color.h"
#include "texture.fwd.h"
#include "font.fwd.h"
#include <suprengine/ecs/components/renderers/renderer.fwd.h>

#include <vector>

namespace suprengine
{
	enum class DrawType
	{
		FILL,
		LINE,
	};

	class RenderBatch
	{
	private:
		Vec2 translation { Vec2::zero };
		SDL_Renderer* sdl_renderer { nullptr };

		std::vector<Renderer*> renderers;

		Color background_color { Color::black };
	public:
		RenderBatch() {};
		RenderBatch( const RenderBatch& ) = delete;
		RenderBatch& operator=( const RenderBatch& ) = delete;
		~RenderBatch();

		bool initialize( Window* window );

		void begin_render();
		void render();
		void end_render();

		void translate( const Vec2& pos ) { translation += pos; }

		void draw_rect( DrawType draw_type, const Rect& rect, const Color& color );
		void draw_texture( const Rect& src_rect, const Rect& dest_rect, const double rotation, const Vec2& origin, Texture* texture, const Color& color );

		void add_renderer( Renderer* renderer );
		void remove_renderer( Renderer* renderer );

		void set_background_color( Color color ) { background_color = color; }

		SDL_Renderer* get_sdl_renderer() const { return sdl_renderer; }
	};
}

