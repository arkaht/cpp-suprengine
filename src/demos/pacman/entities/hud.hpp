#pragma once

#include "../game_stats.hpp"
#include "pacman.h"

#include <suprengine/ecs/components/renderers/text_renderer.hpp>

class HUD : public Entity
{
private:
	const float TEXT_1UP_FLICK_TIME { .225f };

	float text_1up_flick_time { 0.0f };
public:
	GameStats* stats { &GameStats::instance() };

	int score { 0 }, highscore { 0 };

	TextRenderer* text_score { nullptr };
	TextRenderer* text_highscore { nullptr };
	TextRenderer* title_1up { nullptr };

	HUD() : Entity()
	{
		auto camera = game->get_camera();
		auto font = Assets::get_font( "PressStart2P.ttf", 8 );

		auto title_highscore = new TextRenderer( this, font, "HIGH SCORE" );
		title_highscore->dest.x = camera->viewport.w / camera->zoom / 2.0f - title_highscore->dest.w / 4.0f;
		title_highscore->dest.y = -20.0f;

		title_1up = new TextRenderer( this, font, "1UP" );
		title_1up->dest.x = 40.0f;
		title_1up->dest.y = title_highscore->dest.y;

		text_highscore = new TextRenderer( this, font, "00" );
		text_highscore->dest.x = title_highscore->dest.x;
		text_highscore->dest.y = title_highscore->dest.y + 8.0f + 1.0f;

		text_score = new TextRenderer( this, font, "00" );
		text_score->dest.x = title_1up->dest.x;
		text_score->dest.y = text_highscore->dest.y;
	}

	void update_this( float dt ) override
	{
		//  update score text
		if ( stats->score != score )
		{
			score = stats->score;
			text_score->set_text( std::to_string( score ) );
		}

		//  update highscore text
		if ( stats->highscore != highscore )
		{
			highscore = stats->highscore;
			text_highscore->set_text( std::to_string( highscore ) );
		}

		//  flick 1UP text
		if ( ( text_1up_flick_time -= dt ) <= 0.0f )
		{
			title_1up->is_rendered = !title_1up->is_rendered;
			text_1up_flick_time = TEXT_1UP_FLICK_TIME;
		}
	}
};