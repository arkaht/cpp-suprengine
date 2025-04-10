#pragma once
#include <suprengine/ecs/component.h>

#include <suprengine/ecs/entity.h>
#include <suprengine/ecs/components/transform.h>
#include <suprengine/math/vec2.h>

#include "../entities/level.h"

using namespace suprengine;

namespace demo_pacman
{

	class Mover : public Component
	{
	protected:
		Vec2 current_pos { Vec2::zero }, next_pos { Vec2::zero };

		Vec2 end_tunnel_pos { Vec2::zero };
		bool is_in_tunnel { false };
	public:
		bool is_blocked { false };
		Vec2 direction { Vec2::zero };
		float move_time { 0.125f };
		float current_move_time { 0.0f };
		float rotation { 0.0f };
		bool rotate_towards_dir { false };

		Level* level { nullptr };

		Mover( Entity* owner, Level* level, int priority_order = 0 )
			: level( level ), Component( owner, priority_order ) {}

		void update( float dt ) override
		{
			if ( owner->transform->location == next_pos * Level::TILE_SIZE )
			{
				//  tunnel
				if ( is_in_tunnel )
				{
					set_pos( end_tunnel_pos - direction );
					next_pos = current_pos + direction;
					is_in_tunnel = false;
				}
				else
				{
					//  set current pos
					current_pos = next_pos;
					on_next_pos_reached();
				}
			}

			if ( !is_blocked )
			{
				current_move_time += dt;

				while ( current_move_time >= move_time )
				{
					//  move
					Transform* transform = owner->transform;
					transform->set_location( Vec2::approach( transform->location, next_pos * Level::TILE_SIZE, 1.0f ) );

					on_moved();

					//  rotate
					if ( rotate_towards_dir )
					{
						transform->set_rotation( Quaternion( Vec3 { 0.0f, 0.0f, direction.get_angle() } ) );
					}

					//  reset timer
					current_move_time -= move_time;
				}
			}
		}

		bool try_set_dir( Vec2 dir )
		{
			if ( dir == Vec2::zero ) return false;

			Vec2 new_pos = current_pos + dir;

			//  check tunnel
			int tunnel_id = level->get_tunnel_id( new_pos ); 
			if ( tunnel_id > -1 ) 
			{
				is_in_tunnel = true;

				//  target off-screen pos
				new_pos += dir;

				//  get end tunnel pos
				if ( tunnel_id % 2 == 0 )
					end_tunnel_pos = level->get_tunnel( tunnel_id + 1 );
				else
					end_tunnel_pos = level->get_tunnel( tunnel_id - 1 );
			}
			else
			{
				//  check wall collision
				if ( level->is_wall_tile( (int) new_pos.x, (int) new_pos.y ) ) 
					return false;
			}


			//  apply direction
			next_pos = new_pos;
			direction = dir;

			//printf( "curr_x:%d; curr_y:%d\n", current_pos.get_x(), current_pos.get_y() );
			//printf( "dir_x:%d; dir_y:%d\n", dir.get_x(), dir.get_y() );
			//printf( "next_x:%d; next_y:%d\n", next_pos.get_x(), next_pos.get_y() );
			//printf( "x:%i, y:%i\n", direction.get_x(), direction.get_y() );
			return true;
		}

		void set_pos( Vec2 pos )
		{
			owner->transform->set_location(
				Vec2 {
					pos.x * Level::TILE_SIZE,
					pos.y * Level::TILE_SIZE
				}
			);
		
			current_pos = pos, next_pos = pos;
		}

		Vec2 get_next_pos() const { return next_pos; }
		Vec2 get_pos() const { return current_pos; }

		virtual void on_moved() {}
		virtual void on_next_pos_reached() 
		{
			//  try change direction
			is_blocked = false;
			if ( !try_set_dir( get_desired_dir() ) )
			{
				//  continue otherwise
				if ( !try_set_dir( direction ) )
				{
					is_blocked = true;
				}
			}
		}

		virtual Vec2 get_desired_dir()
		{
			return Vec2::right;
		};


		void debug_render( RenderBatch* render_batch ) override
		{
			Vec2 pos = current_pos * Level::TILE_SIZE;
			Vec2 size { Level::TILE_SIZE, Level::TILE_SIZE };

			//  transform pos
			/*render_batch->draw_rect(
				DrawType::FILL,
				Rect { owner->transform->pos, size },
				Color { 255, 0, 0, 54 }
			);*/

			render_batch->draw_rect(
				DrawType::FILL,
				Rect { get_next_pos() * Level::TILE_SIZE, size },
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
}