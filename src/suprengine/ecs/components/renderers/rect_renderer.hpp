#pragma once
#include "renderer.h"

namespace suprengine
{
	class RectRenderer : public Renderer
	{
	public:
		Rect rect;
		DrawType draw_type { DrawType::FILL };

		RectRenderer( Entity* owner, Rect rect, Color modulate = Color::white, int priority_order = 0 )
			: rect( rect ), Renderer( owner, modulate, priority_order ) {}

		void render() override
		{
			Rect render_rect = owner->transform->get_rect( rect );
			render_batch->draw_rect( draw_type, render_rect, modulate );
		}
	};
}