#pragma once

#include <suprengine/usings.h>
#include <suprengine/window.h>
#include <suprengine/rect.h>
#include <suprengine/color.h>
#include <suprengine/texture.fwd.h>
#include <suprengine/mesh.h>
#include <suprengine/font.fwd.h>

#include <suprengine/ecs/components/renderers/renderer.fwd.h>

#include <vector>
#include <unordered_map>

namespace suprengine
{
	enum class FilteringType
	{
		NEAREST,
		BILINEAR,
	};

	struct TextureParams
	{
		FilteringType filtering = FilteringType::NEAREST;
	};

	enum class DrawType
	{
		FILL,
		LINE,
	};

	enum class RenderPhase
	{
		MESH,
		SPRITE,
	};

	class RenderBatch
	{
	protected:
		Window* window;
		std::unordered_map<RenderPhase, std::vector<Renderer*>> renderers;

		Vec2 translation { Vec2::zero };
		Color background_color { Color::black };
	public:
		RenderBatch( Window* window ) : window( window ) {}
		virtual ~RenderBatch() {};

		virtual bool initialize() = 0;
		virtual void begin_render() = 0;
		virtual void render();
		virtual void render_phase( const RenderPhase phase );
		virtual void end_render() = 0;

		virtual void draw_rect( DrawType draw_type, const Rect& rect, const Color& color ) = 0;
		virtual void draw_texture( const Rect& src_rect, const Rect& dest_rect, float rotation, const Vec2& origin, Texture* texture, const Color& color ) = 0;
		virtual void draw_mesh( const Mtx4& matrix, Mesh* mesh, int texture_id = 0, const Color& color = Color::white ) = 0;

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