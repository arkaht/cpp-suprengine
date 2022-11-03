#include "ghost_manager.h"

#include "entities/ghost.hpp"

bool GhostManager::is_in_chase_mode;
std::vector<Ghost*> GhostManager::ghosts;

void GhostManager::update_this( float dt )
{
	if ( ( current_timer -= dt ) <= 0.0f )
	{
		is_in_chase_mode = !is_in_chase_mode;
		printf( is_in_chase_mode ? "CHASE MODE!" : "SCATTER MODE!" );

		//  update ghosts state
		for ( auto ghost : ghosts )
		{
			switch ( ghost->mover->state )
			{
			case GhostState::SCATTER:
			case GhostState::CHASE:
				ghost->mover->set_state( get_current_state() );
				break;
			}
		}

		//  get next timer
		if ( ++current_timer_id < TIMER_LENGTH )
		{
			current_timer = TIMERS[current_timer_id];
			printf( " for %fs..\n", current_timer );
		}
		//  pause this otherwise
		else
		{
			state = EntityState::PAUSED;
			printf( " indefinitely, good luck bro!" );
		}
	}
}

GhostState GhostManager::get_current_state()
{
	return is_in_chase_mode ? GhostState::CHASE : GhostState::SCATTER;
}

void GhostManager::kill_all()
{
	for ( auto ghost : ghosts )
	{
		ghost->kill();
	}
	ghosts.clear();

	printf( "GhostManager: deleted all ghosts\n" );
}

void GhostManager::flee_all()
{
	for ( auto ghost : ghosts )
	{
		switch ( ghost->mover->state )
		{
		case GhostState::CHASE:
		case GhostState::SCATTER:
			ghost->mover->set_state( GhostState::FLEE );
			break;
		case GhostState::FLEE:
			ghost->mover->reset_flee_timer();
			break;
		}
	}

	printf( "GhostManager: fleed all ghosts\n" );
}