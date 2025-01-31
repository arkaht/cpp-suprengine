#pragma once

#include <suprengine/core/render-batch.h>

#include <suprengine/components/camera.h>

#include <suprengine/rendering/vertex-array.h>
#include <suprengine/rendering/model.h>
#include <suprengine/rendering/shader.h>

#include <SDL_image.h>

namespace suprengine
{
	/*
	 * Renderer for OpenGL by setting up SDL.
	 */
	class OpenGLRenderBatch : public RenderBatch
	{
	public:
		OpenGLRenderBatch( Window* window );
		OpenGLRenderBatch( const OpenGLRenderBatch& ) = delete;
		OpenGLRenderBatch& operator=( const OpenGLRenderBatch& ) = delete;
		~OpenGLRenderBatch();

		void init() override;
		bool init_imgui() override;
		void begin_imgui_frame() override;

		void begin_render() override;
		void render() override;
		void end_render() override;

		/*
		 * Called whenever the window is resized.
		 * Also called on engine startup.
		 */
		void on_window_resized( const Vec2& size ) override;

		void draw_rect( 
			DrawType draw_type, 
			const Rect& rect, 
			const Color& color 
		) override;
		
		void draw_texture(
			const Rect& src_rect, 
			const Rect& dest_rect, 
			float rotation, 
			const Vec2& origin,
			SharedPtr<Texture> texture, 
			const Color& color 
		) override;
		void draw_texture(
			const Mtx4& matrix,
			SharedPtr<Texture> texture, 
			const Vec2& origin,
			const Rect& src_rect, 
			const Color& color = Color::white 
		) override;

		void draw_mesh( 
			const Mtx4& matrix,
			Mesh* mesh, 
			int texture_id, 
			const Color& color 
		) override;
		void draw_model( 
			const Mtx4& matrix, 
			const SharedPtr<Model>& model, 
			rconst_str shader_name,
			const Color& color = Color::white
		) override;
		void draw_debug_model( 
			const Mtx4& matrix, 
			const SharedPtr<Model>& model, 
			const Color& color 
		) override;

		void draw_line(
			const Vec3& start,
			const Vec3& end,
			const Color& color = Color::white
		);

		void translate( const Vec2& pos ) override;
		void scale( float zoom ) override;
		void clip( const Rect& region ) override;

		void set_debug_output( bool is_active );
		void set_samples( uint32 samples );
		void update_framebuffers();

		bool set_vsync( VSyncMode mode ) override;
		VSyncMode get_vsync_mode() override;

		uint32 get_samples() const;

	private:
		void _load_assets();

		void _create_framebuffers( int width, int height );
		void _release_framebuffers();

		Mtx4 _compute_location_matrix( float x, float y, float z );
		void _draw_elements( int indices_count );
	
	private:
		Vec2 _viewport_size = Vec2::zero;
		Vec2 _screen_offset = Vec2::zero;

		SDL_GLContext _gl_context = nullptr;

		uint32 _fbo_id = 0;
		uint32 _pp_fbo_id = 0;
		uint32 _rbo_id = 0;
		uint32 _framebuffer_texture_id = 0;
		uint32 _pp_texture_id = 0;

		uint32 _samples = 8;

		VertexArray* _rect_vertex_array = nullptr;
		VertexArray* _quad_vertex_array = nullptr;

		SharedPtr<Shader> _color_shader = nullptr;
		SharedPtr<Shader> _texture_shader = nullptr;
		SharedPtr<Shader> _framebuffer_shader = nullptr;

		Camera* _camera = nullptr;

		Mtx4 _view_matrix;
		Mtx4 _viewport_matrix;

		uint32 _render_tick = 0;
		VSyncMode _vsync_mode = VSyncMode::Disabled;
	};
}

