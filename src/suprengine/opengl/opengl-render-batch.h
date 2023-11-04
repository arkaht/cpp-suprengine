#pragma once
#include <suprengine/render-batch.h>
#include <suprengine/components/camera.h>

#include <SDL_image.h>

#include "vertex-array.h"
#include "shader.h"

namespace suprengine
{
	class OpenGLRenderBatch : public RenderBatch
	{
	private:
		SDL_GLContext gl_context { nullptr };
	
		VertexArray* quad_vertex_array { nullptr };
		Shader* color_shader { nullptr }, *texture_shader { nullptr };

		Camera* camera { nullptr };
		Vec3 screen_offset;

		Mtx4 _view_matrix;

	public:
		OpenGLRenderBatch( Window* _window ) : RenderBatch( _window ) {};
		OpenGLRenderBatch( const OpenGLRenderBatch& ) = delete;
		OpenGLRenderBatch& operator=( const OpenGLRenderBatch& ) = delete;
		~OpenGLRenderBatch();

		bool init() override;

		void begin_render() override;
		void render() override;
		void end_render() override;

		void draw_rect( DrawType draw_type, const Rect& rect, const Color& color ) override;
		void draw_texture( const Rect& src_rect, const Rect& dest_rect, float rotation, const Vec2& origin, Texture* texture, const Color& color ) override;
		void draw_texture( const Mtx4& matrix, Texture* texture, const Vec2& origin, const Rect& src_rect, const Color& color = Color::white ) override;
		void draw_mesh( const Mtx4& matrix, Mesh* mesh, int texture_id, const Color& color ) override;
		
		void scale( float zoom ) override;
		void clip( const Rect& region ) override;

		Texture* load_texture_from_surface( rconst_str path, SDL_Surface* surface, const TextureParams& params = {} ) override;

	private:
		Mtx4 compute_location_matrix( float x, float y, float z );
		void draw_elements( int indices_count );
	};
}

