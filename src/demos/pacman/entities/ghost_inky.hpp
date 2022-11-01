#pragma once

#include "ghost_blinky.hpp"

class GhostInky : public Ghost
{
public:
	GhostBlinky* blinky;

	GhostInky( Level* level, GhostBlinky* blinky, PacMan* pacman )
		: blinky( blinky ), Ghost( level, pacman )
	{
		mover->wait_id = 0;
		mover->wait_time = 5.0f;

		anim->gen_frames( Rect { 0.0f, 48.0f, 128.0f, 16.0f }, Vec2 { 16.0f, 16.0f } );
		
		color = { 0, 255, 255 };
	}

	Vec2 get_scatter_target() override
	{ 
		return { 
			level->get_width() - 1.0f,
			level->get_height() - 1.0f
		}; 
	}

	Vec2 get_chase_target() override
	{
		Vec2 pacman_pos = pacman->mover->get_pos();
		Vec2 target = pacman_pos, dir = pacman->mover->direction;

		//  emulate the integer overflow that Inky had for handling up-vector
		if ( dir == Vec2::up )
		{
			dir.x = -1.0f;
		}

		//  multiply dir by 2
		target += dir * 2.0f;

		Vec2 offset_dir = pacman_pos - blinky->mover->get_pos();
		target += offset_dir;

		return target;
	}
};