#pragma once

#include "mover.hpp"

namespace demo_pacman
{
	class PlayerMover : public Mover
	{
	public:
		Vec2 desired_dir { Vec2::right };

		PlayerMover( Entity* owner, Level* level, int priority_order = 0 )
			: Mover( owner, level, priority_order ) {}

		void update( float dt ) override
		{
			Game* game = owner->get_game();

			//  set desired direction
			if ( game->is_key_pressed( SDL_SCANCODE_W ) || game->is_key_pressed( SDL_SCANCODE_UP ) )
			{
				desired_dir = Vec2::up;
			}
			else if ( game->is_key_pressed( SDL_SCANCODE_S ) || game->is_key_pressed( SDL_SCANCODE_DOWN ) )
			{
				desired_dir = Vec2::down;
			}
			else if ( game->is_key_pressed( SDL_SCANCODE_A ) || game->is_key_pressed( SDL_SCANCODE_LEFT ) )
			{
				desired_dir = Vec2::left;
			}
			else if ( game->is_key_pressed( SDL_SCANCODE_D ) || game->is_key_pressed( SDL_SCANCODE_RIGHT ) )
			{
				desired_dir = Vec2::right;
			}

			//  base update
			Mover::update( dt );
		}

		Vec2 get_desired_dir() override
		{
			return desired_dir;
		}
	};
}