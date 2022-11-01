#pragma once

#include "mover.hpp"

class PlayerMover : public Mover
{
public:
	Int2_16b desired_dir { Int2_16b::right };

	PlayerMover( Entity* owner, Level* level, int priority_order = 0 )
		: Mover( owner, level, priority_order ) {}

	void update( float dt ) override
	{
		Game* game = owner->get_game();

		//  set desired direction
		if ( game->is_key_pressed( SDL_SCANCODE_W ) || game->is_key_pressed( SDL_SCANCODE_UP ) )
		{
			desired_dir = Int2_16b::up;
		}
		else if ( game->is_key_pressed( SDL_SCANCODE_S ) || game->is_key_pressed( SDL_SCANCODE_DOWN ) )
		{
			desired_dir = Int2_16b::down;
		}
		else if ( game->is_key_pressed( SDL_SCANCODE_A ) || game->is_key_pressed( SDL_SCANCODE_LEFT ) )
		{
			desired_dir = Int2_16b::left;
		}
		else if ( game->is_key_pressed( SDL_SCANCODE_D ) || game->is_key_pressed( SDL_SCANCODE_RIGHT ) )
		{
			desired_dir = Int2_16b::right;
		}

		//  base update
		Mover::update( dt );
	}

	Int2_16b get_desired_dir() override
	{
		return desired_dir;
	}
};