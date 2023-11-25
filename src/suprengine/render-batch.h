#pragma once
#include <suprengine/game.fwd.h>

#include <suprengine/usings.h>
#include <suprengine/window.h>
#include <suprengine/rect.h>
#include <suprengine/color.h>
#include <suprengine/texture.fwd.h>
#include <suprengine/model.h>
#include <suprengine/font.fwd.h>

#include <suprengine/components/renderer.fwd.h>

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

	struct AmbientLightInfos
	{
		Color color;
		Vec3 direction;
		float scale;
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
	public:
		RenderBatch( Window* _window );
		virtual ~RenderBatch() {};

		virtual bool init() = 0;
		virtual void begin_render() = 0;
		virtual void render();
		virtual void render_phase( const RenderPhase phase );
		virtual void end_render() = 0;

		virtual void draw_rect( DrawType draw_type, const Rect& rect, const Color& color = Color::white ) = 0;
		virtual void draw_texture( const Rect& src_rect, const Rect& dest_rect, float rotation, const Vec2& origin, Texture* texture, const Color& color = Color::white ) = 0;
		virtual void draw_texture( const Mtx4& matrix, Texture* texture, const Vec2& origin, const Rect& src_rect, const Color& color = Color::white ) = 0;
		virtual void draw_mesh( const Mtx4& matrix, Mesh* mesh, int texture_id = 0, const Color& color = Color::white ) = 0;
		virtual void draw_model( 
			const Mtx4& matrix, 
			Model* model, 
			rconst_str shader_name,
			const Color& color = Color::white 
		) = 0;
		virtual void draw_debug_model( 
			const Mtx4& matrix, 
			Model* model, 
			const Color& color 
		) = 0;

		void translate( const Vec2& pos );
		virtual void scale( float zoom ) = 0;
		virtual void clip( const Rect& region ) = 0;

		virtual Texture* load_texture( rconst_str path, const TextureParams& params = {} );
		virtual Texture* load_texture_from_surface( rconst_str path, SDL_Surface* surface, const TextureParams& params = {} );

		void set_background_color( Color color );

		void set_ambient_direction( const Vec3& direction );
		void set_ambient_scale( float scale );
		void set_ambient_color( Color color );

		void add_renderer( Renderer* renderer );
		void remove_renderer( Renderer* renderer );

	protected:
		Window* _window;
		std::unordered_map<RenderPhase, std::vector<Renderer*>> renderers;

		AmbientLightInfos ambient_light;

		Vec2 translation { Vec2::zero };
		Color background_color { Color::black };
		Game* game { nullptr };
	};
}