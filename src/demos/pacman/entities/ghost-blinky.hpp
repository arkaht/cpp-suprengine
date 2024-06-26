#pragma once

#include "ghost.hpp"

namespace demo_pacman
{
	class GhostBlinky : public Ghost
	{
	public:
		GhostBlinky( Level* level, PacMan* pacman )
			: Ghost( level, pacman ) 
		{
			mover->wait_id = 2;
			mover->wait_time = 0.0f;

			anim->gen_frames( Rect { 0.0f, 16.0f, 128.0f, 16.0f }, Vec2 { 16.0f, 16.0f } );
			color = { 255, 0, 0 };
		}

		Vec2 get_scatter_target() override
		{
			return { 
				level->get_width() - 1.0f, 
				0 
			};
		}

		Vec2 get_chase_target() override
		{
			return pacman->mover->get_pos();
		}
	};
}