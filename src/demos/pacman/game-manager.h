#pragma once
#include "entities/ghost.fwd.h"
#include "entities/pacman.h"

#include <suprengine/ecs/entity.h>

#include <vector>
#include <algorithm>

using namespace suprengine;

namespace demo_pacman
{
	class GameManager : public Entity
	{
	private:
		static const int TIMER_LENGTH { 7 };
		const float TIMERS[TIMER_LENGTH]
		{
			// SCATTER  CHASE
				7.0f,	20.0f,
				7.0f,	20.0f,
				5.0f,	20.0f,
				5.0f,
		};

		static bool is_in_chase_mode;
		static float flee_timer;

		int current_timer_id { 0 };
		float current_timer { 0.0f };
		bool is_timer_running { true };
	public:
		static GameManager* instance;
		static PacMan* pacman;
		static std::vector<Ghost*> ghosts;

		GameManager() 
			: Entity() 
		{
			instance = this;

			is_in_chase_mode = false;
			current_timer = TIMERS[current_timer_id];
		}

		void update_this( float dt ) override;

		static GhostState get_current_state();

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
}