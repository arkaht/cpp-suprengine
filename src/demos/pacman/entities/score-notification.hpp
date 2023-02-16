#pragma once

#include <suprengine/ecs/entity.h>
#include <suprengine/ecs/components/renderers/sprite-renderer.hpp>

#include "ghost.hpp"
#include "../game-manager.h"

using namespace suprengine;

namespace demo_pacman
{
	enum class ComboScore
	{
		SCORE_200 = 200,
		SCORE_400 = 400,
		SCORE_800 = 800,
		SCORE_1600 = 1600,
	};

	class ScoreNotification : public Entity
	{
	private:
		const float LIFE_TIME { 1.0f };
		float life_time { 0.0f };
	public:
		SpriteRenderer* sprite { nullptr };

		ScoreNotification( ComboScore combo ) 
			: Entity()
		{
			transform->pos = GameManager::pacman->mover->get_pos() * Level::TILE_SIZE;
		
			life_time = LIFE_TIME;

			sprite = new SpriteRenderer( this, Assets::get_texture( "atlas.png" ) );
			sprite->source = { 128.0f, 48.0f, 16.0f, 16.0f };
			sprite->origin = Vec2::zero;

			switch ( combo )
			{
			case ComboScore::SCORE_400:
				sprite->source.x += 16.0f;
				break;
			case ComboScore::SCORE_800:
				sprite->source.x += 32.0f;
				break;
			case ComboScore::SCORE_1600:
				sprite->source.x += 48.0f;
				break;
			}

			sprite->dest = { -4.0f, -4.0f, sprite->source.w, sprite->source.h };

			set_game_active( false );
		}

		void set_game_active( bool is_active )
		{
			EntityState state = is_active ? EntityState::ACTIVE : EntityState::PAUSED;

			//  active game
			GameManager::instance->set_state( state );

			//  active pacman
			PacMan* pacman = GameManager::pacman;
			pacman->set_state( state );
			pacman->anim->is_rendered = is_active;

			//  active ghosts
			for ( auto ghost : GameManager::ghosts )
			{
				ghost->set_state( state );
				ghost->anim->is_rendered = true;
			}
		}

		void update_this( float dt ) override
		{
			if ( ( life_time -= dt ) <= 0.0f )
			{
				set_game_active( true );
				kill();
			}
		}
	};
}