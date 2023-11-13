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

		ModelRenderer( Entity* owner, Model* model, int priority_order = 0 )
			: model( model ), Renderer( owner, priority_order ) {}

		void render() override
		{
			_render_batch->draw_model( 
				owner->transform->get_matrix(), 
				model, 
				modulate 
			);
		}

		RenderPhase get_render_phase() const override { return RenderPhase::MESH; }
	};
}