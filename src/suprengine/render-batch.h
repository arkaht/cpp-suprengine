#pragma once

#include <suprengine/usings.h>
#include <suprengine/window.h>
#include <suprengine/rect.h>
#include <suprengine/color.h>
#include <suprengine/texture.fwd.h>
#include <suprengine/mesh.hpp>
#include <suprengine/font.fwd.h>

#include <suprengine/ecs/components/renderers/renderer.fwd.h>

#include <vector>

namespace suprengine
{
	enum FILTERING
	{
		NEAREST,
		BILINEAR,
	};

	struct TextureParams
	{
		FILTERING filtering = FILTERING::NEAREST;
	};

	enum class DrawType
	{
		FILL,
		LINE,
	};

	class RenderBatch
	{
	protected:
		Window* window;
		std::vector<Renderer*> renderers;

		Vec2 translation { Vec2::zero };
		Color background_color { Color::black };
	public:
		RenderBatch( Window* window ) : window( window ) {}
		virtual ~RenderBatch() {};

		virtual bool initialize() = 0;
		virtual void begin_render() = 0;
		void render();
		virtual void end_render() = 0;

		virtual void draw_rect( DrawType draw_type, const Rect& rect, const Color& color ) = 0;
		virtual void draw_texture( const Rect& src_rect, const Rect& dest_rect, float rotation, const Vec2& origin, Texture* texture, const Color& color ) = 0;
		virtual void draw_mesh( const Mtx4& matrix, Mesh* mesh, int texture_id = 0 ) = 0;

		void translate( const Vec2& pos );
		virtual void scale( float zoom ) = 0;
		virtual void clip( const Rect& region ) = 0;

		virtual Texture* load_texture( rconst_str path, const TextureParams& params = {} );
		virtual Texture* load_texture_from_surface( rconst_str filename, SDL_Surface* surface, const TextureParams& params = {} ) = 0;

		void set_background_color( Color color );

		void add_renderer( Renderer* renderer );
		void remove_renderer( Renderer* renderer );
	};
}