#pragma once
#include <suprengine/render-batch.h>

#include <SDL_image.h>

#include "vertex-array.h"
#include "shader.h"

namespace suprengine
{
	class OpenGLRenderBatch : public RenderBatch
	{
	private:
		SDL_GLContext gl_context { nullptr };
	
		VertexArray* vertex_array { nullptr };
		Shader* color_shader { nullptr }, *texture_shader { nullptr };

		Mtx4 view_projection;
		Vec3 screen_offset;
	public:
		OpenGLRenderBatch( Window* window ) : RenderBatch( window ) {};
		OpenGLRenderBatch( const OpenGLRenderBatch& ) = delete;
		OpenGLRenderBatch& operator=( const OpenGLRenderBatch& ) = delete;
		~OpenGLRenderBatch();

		bool initialize() override;

		void begin_render() override;
		void end_render() override;

		void draw_rect( DrawType draw_type, const Rect& rect, const Color& color ) override;
		void draw_texture( const Rect& src_rect, const Rect& dest_rect, const double rotation, const Vec2& origin, Texture* texture, const Color& color ) override;
		
		void scale( float zoom ) override;
		void clip( const Rect& region ) override;

		Texture* load_texture_from_surface( rconst_str path, SDL_Surface* surface ) override;
	private:
		Mtx4 compute_location_matrix( const Vec3 pos );
	};
}

