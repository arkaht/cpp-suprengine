#pragma once
#include "renderer.h"
#include "transform2.hpp"
#include "suprengine/rect.h"

namespace suprengine
{
	class RectRenderer : public Renderer
	{
	public:
		Rect rect;

		RectRenderer( Entity* owner, Rect rect, Color modulate = Color::white, int priority_order = 0 )
			: rect( rect ), Renderer( owner, modulate, priority_order ) {}

		void render() override
		{
			Rect render_rect = owner->transform->get_rect( rect );
			render_batch->draw_rect( render_rect, modulate );
		}
	};
}