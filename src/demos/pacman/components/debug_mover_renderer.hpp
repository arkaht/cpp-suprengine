#pragma once

#include "mover.hpp"
#include <suprengine/ecs/components/renderers/renderer.h>

class DebugMoverRenderer : public Renderer
{
public:
	Mover* mover { nullptr };

	DebugMoverRenderer( Entity* owner, Mover* mover, int priority_order = 0 )
		: mover( mover ), Renderer( owner, Color::white, priority_order ) {}

	void render() override
	{
		Vec2 pos = mover->get_pos().to_vec2() * Level::TILE_SIZE;
		Vec2 size { Level::TILE_SIZE, Level::TILE_SIZE };

		//  transform pos
		/*render_batch->draw_rect(
			DrawType::FILL,
			Rect { owner->transform->pos, size },
			Color { 255, 0, 0, 54 }
		);*/

		render_batch->draw_rect(
			DrawType::FILL,
			Rect { mover->get_next_pos().to_vec2() * Level::TILE_SIZE, size},
			Color { 255, 0, 0, 54 }
		);

		//  grid pos
		render_batch->draw_rect(
			DrawType::FILL,
			Rect { pos, size },
			Color { 0, 255, 0, 54 }
		);

		//  direction pos
		/*render_batch->draw_rect(
			DrawType::FILL,
			Rect { pos + mover->direction.to_vec2() * Level::TILE_SIZE, size },
			Color { 0, 0, 255, 90 }
		);*/
	}
};