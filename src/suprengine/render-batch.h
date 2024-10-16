#pragma once

#include <suprengine/usings.h>
#include <suprengine/window.h>
#include <suprengine/rect.h>
#include <suprengine/color.h>
#include <suprengine/model.h>
#include <suprengine/vec3.h>

#include <suprengine/components/renderer.fwd.h>

#include <vector>
#include <unordered_map>

namespace suprengine
{
	static const std::string SHADER_LIT_MESH = "suprengine::lit-mesh";
	static const std::string TEXTURE_LARGE_GRID = "suprengine::large-grid";
	static const std::string TEXTURE_MEDIUM_GRID = "suprengine::medium-grid";
	static const std::string MESH_ARROW = "suprengine::arrow";
	static const std::string MESH_CUBE = "suprengine::cube";
	static const std::string MESH_CYLINDER = "suprengine::cylinder";
	static const std::string MESH_SPHERE = "suprengine::sphere";

	class Engine;

	enum class FilteringType
	{
		Nearest,
		Bilinear,
	};

	struct TextureParams
	{
		FilteringType filtering = FilteringType::Nearest;
	};

	struct AmbientLightInfos
	{
		Color color = Color::white;
		Vec3 direction = Vec3::zero;
		float scale = 1.0f;
	};

	enum class DrawType
	{
		Fill,
		Line,
	};

	enum class RenderPhase
	{
		//  3D-rendering
		World,
		//  2D-rendering (e.g. Sprites, UI)
		Viewport,
	};

	class RenderBatch
	{
	public:
		RenderBatch( Window* _window );
		virtual ~RenderBatch() {};

		void set_background_color( Color color );

		void set_ambient_direction( const Vec3& direction );
		void set_ambient_scale( float scale );
		void set_ambient_color( Color color );

		void add_renderer( SharedPtr<Renderer> renderer );
		void remove_renderer( SharedPtr<Renderer> renderer );

	public:
		virtual bool init() = 0;

		virtual bool init_imgui() = 0;
		virtual void begin_imgui_frame() = 0;

		virtual void begin_render() = 0;
		virtual void render() = 0;
		virtual void end_render() = 0;

		virtual void on_window_resized( const Vec2& size ) = 0;

		virtual void draw_rect( 
			DrawType draw_type, 
			const Rect& rect,
			const Color& color = Color::white 
		) = 0;
		
		virtual void draw_texture(
			const Vec2& pos,
			const Vec2& scale,
			float rotation,
			const Vec2& origin,
			Texture* texture,
			const Color& color = Color::white
		);
		virtual void draw_texture(
			const Rect& src_rect,
			const Rect& dest_rect, 
			float rotation, 
			const Vec2& origin, 
			Texture* texture,
			const Color& color = Color::white 
		) = 0;
		virtual void draw_texture(
			const Mtx4& matrix,
			Texture* texture,
			const Vec2& origin, 
			const Rect& src_rect, 
			const Color& color = Color::white
		) = 0;

		virtual void draw_mesh( 
			const Mtx4& matrix, 
			Mesh* mesh, 
			int texture_id = 0, 
			const Color& color = Color::white
		) = 0;
		virtual void draw_model( 
			const Mtx4& matrix, 
			const SharedPtr<Model>& model, 
			rconst_str shader_name,
			const Color& color = Color::white 
		) = 0;
		virtual void draw_debug_model( 
			const Mtx4& matrix, 
			const SharedPtr<Model>& model, 
			const Color& color 
		) = 0;

		virtual void translate( const Vec2& pos ) = 0;
		virtual void scale( float zoom ) = 0;
		virtual void clip( const Rect& region ) = 0;

		virtual Texture* load_texture( rconst_str path, const TextureParams& params = {} );
		virtual Texture* load_texture_from_surface( rconst_str path, SDL_Surface* surface, const TextureParams& params = {} );

	protected:
		void _render_phase( RenderPhase phase );

	protected:
		Window* _window;
		std::unordered_map<RenderPhase, std::vector<SharedPtr<Renderer>>> _renderers;

		AmbientLightInfos _ambient_light;

		Color _background_color { Color::black };
	};
}