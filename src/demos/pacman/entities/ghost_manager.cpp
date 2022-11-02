#include "ghost_manager.h"

#include "ghost.hpp"

std::vector<Ghost*> GhostManager::ghosts;

void GhostManager::delete_all()
{
	for ( auto ghost : ghosts )
	{
		delete ghost;
	}
	ghosts.clear();
	printf( "delete all ghosts\n" );
}