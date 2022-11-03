#include "ghost_mover.h"
#include "../entities/ghost.hpp"

GhostMover::GhostMover( Ghost* owner, Level* level, PacMan* pacman, AnimSpriteRenderer* anim, int priority_order )
	: personality( owner ), anim( anim ), pacman( pacman ), Mover( owner, level, priority_order )
{
	move_time = NORMAL_MOVE_TIME;
}

void GhostMover::update( float dt )
{
	//  flee state
	switch ( state )
	{
	case GhostState::FLEE:
		current_flee_time += dt;

		if ( !is_flee_ending )
		{
			if ( current_flee_time >= FLEE_TIME - FLEE_END_TIME )
			{
				is_flee_ending = true;
			}
		}
		else
		{
			if ( current_flee_time >= FLEE_TIME )
			{
				set_state( GhostState::SCATTER );
			}
		}
		break;
	case GhostState::WAIT:
		if ( !is_waiting_finished && ( wait_time -= dt ) <= 0.0f )
		{
			is_waiting_finished = true;
			move_time = SLOW_MOVE_TIME;

			Vec2 door_pos = level->get_ghost_door_pos();

			//  direct exit if same x
			if ( owner->transform->pos.x == door_pos.x )
			{
				next_pos = door_pos;
			}
			//  target entrance height
			else
			{
				next_pos = Vec2 { door_pos.x, current_pos.y };
			}
			direction = ( next_pos - current_pos ).normalize();
		}
		break;
	}

	//  base update
	Mover::update( dt );

	//  anim update
	if ( anim != nullptr )
	{
		switch ( state )
		{
		case GhostState::WAIT:
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
			if ( !is_flee_ending )
				anim->set_current_clip( "flee" );
			else
				anim->set_current_clip( "flee_end" );
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
		target = personality->get_scatter_target();
		break;
	case GhostState::CHASE:
		target = personality->get_chase_target();
		break;
	case GhostState::EATEN:
		target = level->get_ghost_door_pos();
		break;
	}
}

void GhostMover::set_state( GhostState _state )
{
	//  init states
	switch ( _state )
	{
	case GhostState::WAIT:
		move_time = WAIT_MOVE_TIME;
		is_waiting_finished = false;
		next_pos = level->get_wait_pos( wait_id );
		direction = ( next_pos - current_pos ).normalize();
		break;
	case GhostState::FLEE:
		move_time = FLEE_MOVE_TIME;
		reset_flee_timer();
		break;
	case GhostState::EATEN:
		move_time = EATEN_MOVE_TIME;
		break;
	default:
		move_time = NORMAL_MOVE_TIME;
		break;
	}

	//  turn backwards
	if ( state == GhostState::CHASE || state == GhostState::SCATTER )
	{
		try_set_dir( -direction );
	}

	state = _state;
}

void GhostMover::reset_flee_timer()
{
	current_flee_time = 0.0f;
	is_flee_ending = false;
}

void GhostMover::on_next_pos_reached()
{
	switch ( state )
	{
	case GhostState::WAIT:
	{
		if ( !is_waiting_finished )
		{
			Vec2 first_wait_pos = level->get_wait_pos( wait_id );
			Vec2 second_wait_pos = level->get_wait_pos( wait_id + 1 );

			//  patrol between two pos
			if ( current_pos == first_wait_pos )
			{
				next_pos = second_wait_pos;
				direction = ( next_pos - current_pos ).normalize();
				return;
			}
			else if ( current_pos == second_wait_pos )
			{
				next_pos = first_wait_pos;
				direction = ( next_pos - current_pos ).normalize();
				return;
			}
		}
		else
		{
			Vec2 door_pos = level->get_ghost_door_pos();
			Vec2 floored_door_pos = Vec2 {
				floorf( door_pos.x ),
				floorf( door_pos.y ),
			};

			//  at floored door, behave!
			if ( current_pos == floored_door_pos )
			{
				set_state( GhostState::SCATTER );
				return;
			}
			//  at door, fix your pos (so it doesn't offset)
			else if ( current_pos == door_pos )
			{
				next_pos = floored_door_pos;
			}
			//  at door-x, go to door-y
			else if ( current_pos.x == door_pos.x )
			{
				next_pos = door_pos;
			}

			direction = ( next_pos - current_pos ).normalize();
			return;
		}
		break;
	}
	case GhostState::EATEN:
		//  enter the house
		if ( current_pos == target )
		{
			next_pos = level->get_ghost_house_pos();
			direction = ( next_pos - current_pos ).normalize();
			return;
		}
		//  exit state on entered
		else if ( current_pos == level->get_ghost_house_pos() )
		{
			set_state( GhostState::SCATTER );
			return;
		}
		break;
	}

	Mover::on_next_pos_reached();
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
		if ( level->is_wall_tile( (int) ( current_pos.x + dir.x ), (int) ( current_pos.y + dir.y ) ) ) continue;

		//  add possibility
		possibilities.push_back( dir );
	}

	//  the choosen one?!
	Vec2 desired { Vec2::up };

	int length = (int) possibilities.size();
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

	Mover::debug_render( render_batch );
}