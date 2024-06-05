#pragma once
#include "renderer.fwd.h"

#include <suprengine/entity.h>
#include <suprengine/components/transform.h>
#include <suprengine/render-batch.h>

namespace suprengine
{
	class Renderer : public Component
	{
	public:
		Renderer( 
			Color modulate = Color::white, 
			int priority_order = 0 
		);

		virtual void setup() override;
		virtual void unsetup() override;

		virtual void render( RenderBatch* render_batch ) = 0;

		virtual RenderPhase get_render_phase() const = 0;

	public:
		Color modulate { Color::white };
	};
}