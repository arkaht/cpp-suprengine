#include "ghost_mover.h"
#include "../entities/ghost.hpp"

GhostMover::GhostMover( Ghost* owner, Level* level, PacMan* pacman, AnimSpriteRenderer* anim, int priority_order )
	: personality( owner ), anim( anim ), pacman( pacman ), Mover( owner, level, priority_order )
{}

void GhostMover::update( float dt )
{
	//  base update
	Mover::update( dt );

	//  anim update
	if ( anim != nullptr )
	{
		switch ( state )
		{
		case GhostState::CHASE:
		case GhostState::SCATTER:
			if ( direction == Vec2::left )
				anim->set_current_clip( "left" );
			else if ( direction == Vec2::right )
				anim->set_current_clip( "right" );
			else if ( direction == Vec2::up )
				anim->set_current_clip( "up" );
			else if ( direction == Vec2::down )
				anim->set_current_clip( "down" );

			break;
		case GhostState::FLEE:
			anim->set_current_clip( "flee" );
			break;
		case GhostState::EATEN:
			if ( direction == Vec2::left )
				anim->set_current_clip( "eaten_left" );
			else if ( direction == Vec2::right )
				anim->set_current_clip( "eaten_right" );
			else if ( direction == Vec2::up )
				anim->set_current_clip( "eaten_up" );
			else if ( direction == Vec2::down )
				anim->set_current_clip( "eaten_down" );

			break;
		}
	}
}

void GhostMover::update_target()
{
	switch ( state )
	{
	case GhostState::SCATTER:
		//uint8_t max_x = level->get_width() - 1, max_y = level->get_height() - 1;
		target = personality->get_scatter_target();

		/*switch ( personality )
		{
		case GhostPersonality::BLINKY:
			target = { (uint8_t)( max_x - 2 ), 0 };
			break;
		case GhostPersonality::PINKY:
			target = { 2, 0 };
			break;
		case GhostPersonality::CLYDE:
			target = { 0, max_y };
			break;
		case GhostPersonality::INKY:
			target = { max_x, max_y };
			break;
		}*/
		break;
	case GhostState::CHASE:
		target = personality->get_chase_target();
		break;
	}
}

Vec2 GhostMover::get_desired_dir()
{
	//  update target
	update_target();

	//  init directions
	Vec2 directions[4] {
		Vec2::up,
		Vec2::down,
		Vec2::left,
		Vec2::right
	};

	//  evaluate possibilities
	std::vector<Vec2> possibilities;
	Vec2 backward = -direction;
	for ( int i = 0; i < 4; i++ )
	{
		const Vec2& dir = directions[i];

		//  remove backward
		if ( dir == backward ) continue;
		//  remove walls
		if ( level->is_wall_tile( current_pos.x + dir.x, current_pos.y + dir.y ) ) continue;

		//  add possibility
		possibilities.push_back( dir );
	}

	//  the choosen one?!
	Vec2 desired { Vec2::up };

	int length = possibilities.size();
	if ( length > 1 )
	{
		//  flee is a random choice
		if ( state == GhostState::FLEE )
		{
			desired = possibilities[rand() % length];
		}
		//  choose the nearest direction from target
		else
		{
			float nearest_dist = std::numeric_limits<float>::max();

			for ( const Vec2& dir : possibilities )
			{
				Vec2 pos = current_pos + dir;

				float dist = ( pos - target ).length_sqr();
				if ( dist < nearest_dist )
				{
					desired = dir;
					nearest_dist = dist;
				}
			}
		}
	}
	else if ( length == 1 )
	{
		desired = possibilities[0];
	}

	return desired;
}


void GhostMover::debug_render( RenderBatch* render_batch )
{
	Vec2 target_pos = target * Level::TILE_SIZE;
	Vec2 cross_size { Level::TILE_SIZE / 4, Level::TILE_SIZE };

	//  get color
	Color color {
		personality->color.r,
		personality->color.g,
		personality->color.b,
		127
	};

	//  draw cross
	render_batch->draw_rect(
		DrawType::FILL,
		{
			target_pos.x,
			target_pos.y + Level::TILE_SIZE / 2.0f - cross_size.x / 2.0f,
			cross_size.y,
			cross_size.x
		},
		color
	);
	render_batch->draw_rect(
		DrawType::FILL,
		{
			target_pos.x + Level::TILE_SIZE / 2.0f - cross_size.x / 2.0f,
			target_pos.y,
			cross_size.x,
			cross_size.y
		},
		color
	);
}