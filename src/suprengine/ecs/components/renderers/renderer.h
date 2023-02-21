#pragma once
#include "renderer.fwd.h"

#include <suprengine/ecs/entity.h>
#include <suprengine/ecs/components/transform.h>
#include <suprengine/render-batch.h>

namespace suprengine
{
	class Renderer : public Component
	{
	protected:
		RenderBatch* render_batch { nullptr };
	public:
		bool should_render { true };
		Color modulate { Color::white };

		Renderer( Entity* owner, int priority_order = 0 );
		~Renderer() override;

		virtual void init();
		virtual void render() {};

		virtual RenderPhase get_render_phase() const { return RenderPhase::SPRITE; }
	};
}