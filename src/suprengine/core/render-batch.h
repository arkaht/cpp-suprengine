#pragma once

#include <suprengine/core/window.h>

#include <suprengine/math/vec3.h>
#include <suprengine/math/rect.h>
#include <suprengine/math/color.h>

#include <suprengine/rendering/model.h>

#include <suprengine/utils/usings.h>

#include <vector>
#include <unordered_map>

namespace suprengine
{
	static inline const char* const SHADER_LIT_MESH = "suprengine::lit-mesh";

	static inline const char* const TEXTURE_WHITE = "suprengine::white";
	static inline const char* const TEXTURE_LARGE_GRID = "suprengine::large-grid";
	static inline const char* const TEXTURE_MEDIUM_GRID = "suprengine::medium-grid";

	static inline const char* const MESH_ARROW = "suprengine::arrow";
	static inline const char* const MESH_CUBE = "suprengine::cube";
	static inline const char* const MESH_CYLINDER = "suprengine::cylinder";
	static inline const char* const MESH_SPHERE = "suprengine::sphere";
	static inline const char* const MESH_PLANE = "suprengine::plane";

	class Engine;
	class Renderer;

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

	enum class VSyncMode
	{
		Disabled,
		Enabled,
		Adaptative,
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
		AmbientLightInfos get_ambient_infos() const;

		void add_renderer( SharedPtr<Renderer> renderer );
		void remove_renderer( SharedPtr<Renderer> renderer );

	public:
		virtual void init();
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
			SharedPtr<Texture> texture,
			const Color& color = Color::white
		);
		virtual void draw_texture(
			const Rect& src_rect,
			const Rect& dest_rect, 
			float rotation, 
			const Vec2& origin, 
			SharedPtr<Texture> texture,
			const Color& color = Color::white 
		) = 0;
		virtual void draw_texture(
			const Mtx4& matrix,
			SharedPtr<Texture> texture,
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

		virtual void draw_line(
			const Vec3& start,
			const Vec3& end,
			const Color& color = Color::white
		) = 0;

		virtual void translate( const Vec2& pos ) = 0;
		virtual void scale( float zoom ) = 0;
		virtual void clip( const Rect& region ) = 0;

		virtual bool set_vsync( VSyncMode mode ) = 0;
		virtual VSyncMode get_vsync_mode() = 0;

		virtual SharedPtr<Texture> load_texture(
			rconst_str path,
			const TextureParams& params = {}
		);
		virtual SharedPtr<Texture> load_texture_from_surface(
			rconst_str path,
			SDL_Surface* surface,
			const TextureParams& params = {}
		);

		int get_renderers_count( RenderPhase phase ) const;

	protected:
		void _render_phase( RenderPhase phase );

	protected:
		Window* _window;
		std::unordered_map<RenderPhase, std::vector<SharedPtr<Renderer>>> _renderers;

		AmbientLightInfos _ambient_light;

		Color _background_color { Color::black };
	};
}