#pragma once

#include "ghost.hpp"

class GhostPinky : public Ghost
{
public:
	GhostPinky( Level* level, PacMan* pacman )
		: Ghost( level, pacman )
	{
		anim->gen_frames( Rect { 0.0f, 32.0f, 128.0f, 16.0f }, Vec2 { 16.0f, 16.0f } );
		color = { 255, 183, 255 };
	}

	Vec2 get_scatter_target() override { return Vec2::zero; }

	Vec2 get_chase_target() override
	{
		Vec2 dir = pacman->mover->direction;
		Vec2 target = pacman->mover->get_pos();
		
		//  emulate the integer overflow that Pinky had for handling up-vector
		if ( dir == Vec2::up )
		{
			dir.x = -1.0f;
		}

		//  multiply dir by 4
		target += dir * 4.0f;

		return target;
	}
};