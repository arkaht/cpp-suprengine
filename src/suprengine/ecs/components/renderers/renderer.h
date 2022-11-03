#pragma once
#include "renderer.fwd.h"

#include <suprengine/ecs/entity.h>
#include <suprengine/ecs/components/transform2.hpp>
#include <suprengine/render_batch.h>

namespace suprengine
{
	class Renderer : public Component
	{
	protected:
		RenderBatch* render_batch;
	public:
		bool is_rendered { true };
		Color modulate { Color::white };

		Renderer( Entity* owner, Color modulate = Color::white, int priority_order = 0 );
		~Renderer() override;

		virtual void render() {};
	};
}