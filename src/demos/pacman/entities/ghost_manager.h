#pragma once
#include "ghost.fwd.h"

#include <vector>

class GhostManager
{
public:
	static std::vector<Ghost*> ghosts;

	GhostManager() = delete;

	static void add_ghost( Ghost* ghost )
	{
		ghosts.push_back( ghost );
	}

	static void remove_ghost( Ghost* ghost )
	{
		auto itr = std::find( ghosts.begin(), ghosts.end(), ghost );
		if ( itr == ghosts.end() ) return;

		std::iter_swap( itr, ghosts.end() - 1 );
		ghosts.pop_back();
	}

	static void kill_all();

	static void flee_all();
};