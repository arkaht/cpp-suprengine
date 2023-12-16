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

		void scale( float zoom ) override;
		void clip( const Rect& region ) override;

	private:
		Mtx4 compute_location_matrix( float x, float y, float z );
		void draw_elements( int indices_count );
	
		SDL_GLContext gl_context { nullptr };

		VertexArray* quad_vertex_array { nullptr };

		Shader* color_shader { nullptr };
		Shader* texture_shader { nullptr };

		Camera* camera { nullptr };
		Vec3 screen_offset;

		Mtx4 _view_matrix;
		Mtx4 _viewport_matrix;
	};
}

