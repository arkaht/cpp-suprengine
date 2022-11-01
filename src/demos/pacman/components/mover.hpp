#pragma once
#include <suprengine/ecs/component.h>

#include <suprengine/ecs/entity.h>
#include <suprengine/ecs/components/transform2.hpp>
#include <suprengine/vec2.h>

#include "../entities/level.hpp"
#include "../int2_16b.h"

using namespace suprengine;

class Mover : public Component
{
private:
	Int2_16b current_pos { 0 }, next_pos { 0 };
public:
	Int2_16b direction { Int2_16b::right };
	float move_time { 0.125f };
	float current_move_time { 0.0f };
	float rotation { 0.0f };
	bool rotate_towards_dir { false };

	Level* level { nullptr };

	Mover( Entity* owner, Level* level, int priority_order = 0 )
		: level( level ), Component( owner, priority_order ) {}

	void update( float dt ) override
	{
		if ( owner->transform->pos == next_pos.to_vec2() * Level::TILE_SIZE )
		{
			//  set current pos
			current_pos = next_pos;

			//  try change direction
			if ( !try_set_dir( get_desired_dir() ) )
			{
				//  continue otherwise
				if ( !try_set_dir( direction ) )
				{
					direction.value = 0;
				}
			}
		}

		if ( direction.value != 0 && ( current_move_time += dt ) >= move_time )
		{
			//  move
			Transform2* transf = owner->transform;
			transf->pos = transf->pos.approach( next_pos.to_vec2() * Level::TILE_SIZE, 1.0f );

			//  rotate
			if ( rotate_towards_dir )
			{
				transf->rotation = direction.get_angle();
			}

			//  reset timer
			current_move_time = 0.0f;
		}
	}

	bool try_set_dir( Int2_16b dir )
	{
		Int2_16b new_pos = current_pos;

		//  applying direction
		//  NOTE: it's a workaround due of Int2_16b overflow for up-vector :'(
		if ( dir.get_y() != 0 )
		{
			new_pos = Int2_16b(
				new_pos.get_x(),
				new_pos.get_y() + dir.get_y()
			);
		}
		else
		{
			new_pos += dir;
		}

		//  check wall collision
		if ( level->is_wall_tile( new_pos.get_x(), new_pos.get_y() ) ) return false;

		//  apply direction
		next_pos = new_pos;
		direction = dir;

		//printf( "curr_x:%d; curr_y:%d\n", current_pos.get_x(), current_pos.get_y() );
		//printf( "dir_x:%d; dir_y:%d\n", dir.get_x(), dir.get_y() );
		//printf( "next_x:%d; next_y:%d\n", next_pos.get_x(), next_pos.get_y() );
		//printf( "x:%i, y:%i\n", direction.get_x(), direction.get_y() );
		return true;
	}

	void set_pos( Int2_16b pos )
	{
		owner->transform->pos.x = pos.get_x() * Level::TILE_SIZE;
		owner->transform->pos.y = pos.get_y() * Level::TILE_SIZE;
		
		current_pos = pos, next_pos = pos;
	}

	Int2_16b get_next_pos() const { return next_pos; }
	Int2_16b get_pos() const { return current_pos; }

	virtual Int2_16b get_desired_dir() 
	{
		return Int2_16b::right;
	};


	void debug_render( RenderBatch* render_batch ) override
	{
		Vec2 pos = get_pos().to_vec2() * Level::TILE_SIZE;
		Vec2 size { Level::TILE_SIZE, Level::TILE_SIZE };

		//  transform pos
		/*render_batch->draw_rect(
			DrawType::FILL,
			Rect { owner->transform->pos, size },
			Color { 255, 0, 0, 54 }
		);*/

		render_batch->draw_rect(
			DrawType::FILL,
			Rect { get_next_pos().to_vec2() * Level::TILE_SIZE, size },
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