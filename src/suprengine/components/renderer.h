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
		bool should_render { true };
		Color modulate { Color::white };

		Renderer( Entity* owner, Color modulate = Color::white, int priority_order = 0 );

		virtual void setup() override;
		virtual void unsetup() override;

		virtual void render() = 0;

		virtual RenderPhase get_render_phase() const { return RenderPhase::SPRITE; }
	
	protected:
		RenderBatch* _render_batch { nullptr };
	};
}