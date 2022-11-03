#include "ghost_manager.h"

#include "entities/ghost.hpp"

std::vector<Ghost*> GhostManager::ghosts;

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