#include "debug_level_renderer.h"
#include "../entities/level.hpp"

DebugLevelRenderer::DebugLevelRenderer( Level* owner, Color modulate, int priority_order )
	: level( owner ), Renderer( owner, modulate, priority_order ) {}

void DebugLevelRenderer::render()
{
	float tile_size = Level::TILE_SIZE;

	for ( int ty = 0; ty < level->get_height(); ty++ )
	{
		for ( int tx = 0; tx < level->get_width(); tx++ )
		{
			Rect rect {
				tx * tile_size,
				ty * tile_size,
				tile_size,
				tile_size,
			};

			//  draw grid
			render_batch->draw_rect( DrawType::LINE, rect, Color { 255, 255, 255, 27 } );

			//  draw wall
			if ( level->is_wall_tile( tx, ty ) )
			{
				render_batch->draw_rect( DrawType::FILL, rect, modulate );
			}
		}
	}
}
