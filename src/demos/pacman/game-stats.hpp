#pragma once

#include "entities/score-notification.hpp"

namespace demo_pacman
{
	class GameStats
	{
	public:
		static GameStats& instance()
		{
			static GameStats stats;
			return stats;
		}

		int score { 0 };
		int highscore { 0 };
		int remaining_dots { 0 };
		int eat_ghost_combo { 0 };

		void add_score( int amount )
		{
			score += amount;

			if ( score > highscore )
				highscore = score;
		}

		void add_ghost_score( Ghost* ghost )
		{
			//  get combo score
			ComboScore combo;
			switch ( eat_ghost_combo++ )
			{
			case 0:
				combo = ComboScore::SCORE_200;
				break;
			case 1:
				combo = ComboScore::SCORE_400;
				break;
			case 2:
				combo = ComboScore::SCORE_800;
				break;
			default:
				combo = ComboScore::SCORE_1600;
				break;
			}

			//  add score
			add_score( (int) combo );

			//  add notification
			auto notif = new ScoreNotification( combo );

			//  disable eaten ghost rendering
			ghost->anim->is_rendered = false;
		}
	};
}