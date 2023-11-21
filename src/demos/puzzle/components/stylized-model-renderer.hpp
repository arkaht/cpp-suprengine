#pragma once

#include <suprengine/components/renderers/model-renderer.hpp>

namespace puzzle
{
	using namespace suprengine;

	class StylizedModelRenderer : public ModelRenderer
	{
	public:
		float outline_scale = 0.025f;
		bool draw_only_outline = false;

		StylizedModelRenderer(
			Entity* owner,
			Model* model,
			Color modulate = Color::white,
			int priority_order = 0
		)
			: ModelRenderer( owner, model, "stylized", modulate, priority_order ) {}

		void render() override
		{
			Mtx4 outline_matrix = 
				Mtx4::create_scale( transform->scale * ( 1.0f + outline_scale ) )
				* Mtx4::create_from_quaternion( transform->rotation )
				* Mtx4::create_translation( transform->location );

			//  draw outline
			glFrontFace( GL_CCW );
			_render_batch->draw_model(
				outline_matrix,
				model,
				shader_name,
				modulate
			);

			//  draw mesh on top of outline
			if ( !draw_only_outline )
			{
				glFrontFace( GL_CW );
				_render_batch->draw_model(
					transform->get_matrix(),
					model,
					shader_name,
					Color::black
				);
			}
		}
	};
}