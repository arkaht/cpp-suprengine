#pragma once

#include <suprengine/core/render-batch.h>

#include <suprengine/components/transform.h>

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