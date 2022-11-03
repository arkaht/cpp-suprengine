#include "game_manager.h"

#include "entities/ghost.hpp"
#include "game_stats.hpp"

bool GameManager::is_in_chase_mode { false };
float GameManager::flee_timer { 0.0f };

GameManager* GameManager::instance;
std::vector<Ghost*> GameManager::ghosts;
PacMan* GameManager::pacman { nullptr };

void GameManager::update_this( float dt )
{
	if ( is_timer_running )
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
				is_timer_running = false;
				printf( " indefinitely, good luck bro!" );
			}
		}
	}

	//  reset combo timer
	if ( flee_timer > 0.0f && ( flee_timer -= dt ) <= 0.0f )
	{
		GameStats& stats = GameStats::instance();
		stats.eat_ghost_combo = 0;
		printf( "GameManager: reset eat ghost combo\n" );
	}
}

GhostState GameManager::get_current_state()
{
	return is_in_chase_mode ? GhostState::CHASE : GhostState::SCATTER;
}

void GameManager::kill_all()
{
	for ( auto ghost : ghosts )
	{
		ghost->kill();
	}
	ghosts.clear();

	printf( "GhostManager: deleted all ghosts\n" );
}

void GameManager::flee_all()
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

	//  set flee timer
	flee_timer = ghosts[0]->mover->FLEE_TIME;

	printf( "GhostManager: fleed all ghosts\n" );
}