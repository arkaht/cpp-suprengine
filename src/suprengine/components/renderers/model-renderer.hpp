#pragma once
#include <suprengine/components/renderer.h>

#include <suprengine/model.h>

namespace suprengine
{
	class ModelRenderer : public Renderer
	{
	public:
		Model* model { nullptr };
		int texture_id { 0 };
		std::string shader_name;

		ModelRenderer( 
			Entity* owner, 
			Model* model, 
			rconst_str shader_name = "simple-mesh", 
			Color modulate = Color::white,
			int priority_order = 0 
		)
			: model( model ), shader_name( shader_name ),
			  Renderer( owner, modulate, priority_order ) {}

		void render() override
		{
			_render_batch->draw_model( 
				owner->transform->get_matrix(), 
				model, 
				shader_name,
				modulate
			);
		}

		RenderPhase get_render_phase() const override { return RenderPhase::MESH; }
	};
}