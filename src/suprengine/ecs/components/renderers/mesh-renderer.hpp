#pragma once
#include <suprengine/ecs/components/renderers/renderer.h>

#include <suprengine/mesh.hpp>

namespace suprengine
{
	class MeshRenderer : public Renderer
	{
	public:
		Mesh* mesh { nullptr };
		int texture_id { 0 };

		MeshRenderer( Entity* owner, int priority_order = 0 )
			: Renderer( owner, priority_order ) {}

		void render() override
		{
			if ( mesh == nullptr ) return;
			render_batch->draw_mesh( owner->transform->get_matrix(), mesh, texture_id );
		}
	};
}