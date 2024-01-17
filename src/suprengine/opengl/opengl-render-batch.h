#pragma once
#include <suprengine/render-batch.h>
#include <suprengine/components/camera.h>
#include <suprengine/vertex-array.h>
#include <suprengine/model.h>
#include <suprengine/shader.h>

#include <SDL_image.h>

namespace suprengine
{
	class OpenGLRenderBatch : public RenderBatch
	{
	public:
		OpenGLRenderBatch( Window* _window ) : RenderBatch( _window ) {};
		OpenGLRenderBatch( const OpenGLRenderBatch& ) = delete;
		OpenGLRenderBatch& operator=( const OpenGLRenderBatch& ) = delete;
		~OpenGLRenderBatch();

		bool init() override;

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
			Texture* texture, 
			const Color& color 
		) override;
		void draw_texture(
			const Mtx4& matrix,
			Texture* texture, 
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
			Model* model, 
			rconst_str shader_name,
			const Color& color = Color::white
		) override;
		virtual void draw_debug_model( 
			const Mtx4& matrix, 
			Model* model, 
			const Color& color 
		) override;

		void translate( const Vec2& pos ) override;
		void scale( float zoom ) override;
		void clip( const Rect& region ) override;

	private:
		void _load_assets();

		void _create_framebuffers( int width, int height );
		void _release_framebuffers();

		Mtx4 _compute_location_matrix( float x, float y, float z );
		void _draw_elements( int indices_count );
	
	private:
		SDL_GLContext _gl_context { nullptr };

		unsigned int _fbo = 0, _pp_fbo = 0, _rbo = 0;
		unsigned int _framebuffer_texture = 0, _pp_texture = 0;

		VertexArray* _rect_vertex_array { nullptr };
		VertexArray* _quad_vertex_array { nullptr };

		Shader* _color_shader { nullptr };
		Shader* _texture_shader { nullptr };
		Shader* _framebuffer_shader { nullptr };

		Camera* _camera { nullptr };
		Vec3 _screen_offset;

		Mtx4 _view_matrix;
		Mtx4 _viewport_matrix;
	};
}

