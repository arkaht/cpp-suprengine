#pragma once
#include "renderer.fwd.h"

#include <suprengine/ecs/entity.h>
#include <suprengine/ecs/components/transform.h>
#include <suprengine/render-batch.h>

namespace suprengine
{
	class Renderer : public Component
	{
	public:
		bool should_render { true };
		Color modulate { Color::white };

		Renderer( Entity* owner, int priority_order = 0 );

		void setup();
		void unsetup();

		virtual void render() = 0;

		virtual RenderPhase get_render_phase() const { return RenderPhase::SPRITE; }
	
	protected:
		RenderBatch* _render_batch { nullptr };
	};
}