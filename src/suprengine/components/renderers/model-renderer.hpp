#pragma once
#include <suprengine/components/renderer.h>

#include <suprengine/model.h>

namespace suprengine
{
	class ModelRenderer : public Renderer
	{
	public:
		ModelRenderer( 
			SharedPtr<Model> model, 
			rconst_str shader_name = SHADER_LIT_MESH, 
			Color modulate = Color::white,
			int priority_order = 0 
		)
			: model( model ), shader_name( shader_name ),
			  Renderer( modulate, priority_order ) {}

		void render( RenderBatch* render_batch ) override;

		RenderPhase get_render_phase() const override 
		{ 
			return RenderPhase::World; 
		}

	public:
		SharedPtr<Model> model = nullptr;
		int texture_id = 0;
		std::string shader_name {};
	};
}