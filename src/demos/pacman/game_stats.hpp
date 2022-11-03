#pragma once

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

	void add_score( int amount )
	{
		score += amount;

		if ( score > highscore )
			highscore = score;
	}
};