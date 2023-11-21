#pragma once

#include <suprengine/components/renderers/model-renderer.hpp>

namespace puzzle
{
	using namespace suprengine;

	class StylizedModelRenderer : public ModelRenderer
	{
	public:
		StylizedModelRenderer(
			Entity* owner,
			Model* model,
			Color modulate = Color::white,
			int priority_order = 0
		)
			: ModelRenderer( owner, model, "stylized", modulate, priority_order ) {}

		void render() override
		{
			glFrontFace( GL_CW );
			_render_batch->draw_model(
				Mtx4::create_scale( transform->scale * 1.025f )
				* Mtx4::create_from_quaternion( transform->rotation )
				* Mtx4::create_translation( transform->location ),
				model,
				shader_name,
				modulate
			);

			glFrontFace( GL_CCW );
			_render_batch->draw_model(
				transform->get_matrix(),
				model,
				shader_name,
				Color::black
			);
		}
	};
}