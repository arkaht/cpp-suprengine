#pragma once
#include <suprengine/components/renderer.h>

#include <suprengine/model.h>

namespace suprengine
{
	class ModelRenderer : public Renderer
	{
	public:
		ModelRenderer( 
			shared_ptr<Model> model, 
			rconst_str shader_name = SHADER_LIT_MESH, 
			Color modulate = Color::white,
			int priority_order = 0 
		)
			: model( model ), shader_name( shader_name ),
			  Renderer( modulate, priority_order ) {}

		void render( RenderBatch* render_batch ) override
		{
			glFrontFace( GL_CW );
			render_batch->draw_model( 
				transform->get_matrix(), 
				model, 
				shader_name,
				modulate
			);
		}

		RenderPhase get_render_phase() const override 
		{ 
			return RenderPhase::World; 
		}

	public:
		shared_ptr<Model> model { nullptr };
		int texture_id { 0 };
		std::string shader_name;
	};
}