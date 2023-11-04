#pragma once
#include <suprengine/components/renderer.h>

#include <suprengine/opengl/opengl-model.hpp>

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
			if ( model == nullptr ) return;

			auto& matrix = owner->transform->get_matrix();
			for ( int i = 0; i < model->get_mesh_count(); i++ )
			{
				auto mesh = model->get_mesh( i );
				_render_batch->draw_mesh( 
					matrix, 
					mesh, 
					texture_id, 
					modulate 
				);
			}
		}

		RenderPhase get_render_phase() const override { return RenderPhase::MESH; }
	};
}