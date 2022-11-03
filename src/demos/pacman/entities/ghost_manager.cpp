#include "ghost_manager.h"

#include "ghost.hpp"

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
		ghost->mover->set_state( GhostState::FLEE );
	}

	printf( "GhostManager: fleed all ghosts\n" );
}