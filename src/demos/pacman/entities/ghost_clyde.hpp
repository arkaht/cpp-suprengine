#pragma once

#include "ghost.hpp"

class GhostClyde : public Ghost
{
private:
	const float DIST_SQR = powf( 8.0f, 2.0f );
public:
	GhostClyde( Level* level, PacMan* pacman )
		: Ghost( level, pacman )
	{
		anim->gen_frames( Rect { 0.0f, 64.0f, 128.0f, 16.0f }, Vec2 { 16.0f, 16.0f } );
		color = { 255, 183, 31 };
	}

	Vec2 get_scatter_target() override
	{
		return {
			0.0f,
			level->get_height() - 1.0f
		};
	}

	Vec2 get_chase_target() override
	{
		Vec2 target = pacman->mover->get_pos();

		if ( ( mover->get_pos() - target ).length_sqr() > DIST_SQR )
		{
			return target;
		}

		return get_scatter_target();
	}
};