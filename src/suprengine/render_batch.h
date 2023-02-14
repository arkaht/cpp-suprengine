#pragma once

#include <suprengine/window.h>
#include <suprengine/rect.h>
#include <suprengine/color.h>
#include <suprengine/texture.fwd.h>
#include <suprengine/font.fwd.h>

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
	protected:
		std::vector<Renderer*> renderers;

		Vec2 translation { Vec2::zero };
		Color background_color { Color::black };
	public:
		virtual ~RenderBatch() {};

		virtual bool initialize( Window* window ) = 0;
		virtual void begin_render() = 0;
		void render();
		virtual void end_render() = 0;

		virtual void draw_rect( DrawType draw_type, const Rect& rect, const Color& color ) = 0;
		virtual void draw_texture( const Rect& src_rect, const Rect& dest_rect, const double rotation, const Vec2& origin, Texture* texture, const Color& color ) = 0;
	
		void translate( const Vec2& pos );
		virtual void scale( float zoom ) = 0;
		virtual void clip( const Rect& region ) = 0;

		virtual SDL_Texture* load_texture_from_surface( SDL_Surface* surface ) = 0;

		void set_background_color( Color color );

		void add_renderer( Renderer* renderer );
		void remove_renderer( Renderer* renderer );
	};
}