#pragma once
#include <suprengine/components/renderer.h>

#include <suprengine/mesh.h>

namespace suprengine
{
	/*
	 * Obsolete: use ModelRenderer
	 */
	class MeshRenderer : public Renderer
	{
	public:
		Mesh* mesh { nullptr };
		int texture_id { 0 };

		MeshRenderer( 
			shared_ptr<Entity> owner, 
			Mesh* mesh,
			Color modulate = Color::white,
			int priority_order = 0
		)
			: mesh( mesh ), 
			  Renderer( owner, modulate, priority_order )
		{}

		void render() override
		{
			if ( mesh == nullptr ) return;
			_render_batch->draw_mesh( transform->get_matrix(), mesh, texture_id, modulate );
		}

		RenderPhase get_render_phase() const override 
		{ 
			return RenderPhase::World; 
		}
	};
}