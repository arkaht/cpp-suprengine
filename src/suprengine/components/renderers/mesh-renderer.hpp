#pragma once
#include <suprengine/components/renderer.h>

#include <suprengine/mesh.h>

namespace suprengine
{
	class MeshRenderer : public Renderer
	{
	public:
		Mesh* mesh { nullptr };
		int texture_id { 0 };

		MeshRenderer( Entity* owner, Mesh* mesh, int priority_order = 0 )
			: mesh( mesh ), Renderer( owner, priority_order ) {}

		void render() override
		{
			if ( mesh == nullptr ) return;
			_render_batch->draw_mesh( owner->transform->get_matrix(), mesh, texture_id, modulate );
		}

		RenderPhase get_render_phase() const override { return RenderPhase::MESH; }
	};
}