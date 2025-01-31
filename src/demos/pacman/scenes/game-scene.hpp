#pragma once

#include <suprengine/core/scene.h>

#include "../entities/ghost-blinky.hpp"
#include "../entities/ghost-pinky.hpp"
#include "../entities/ghost-inky.hpp"
#include "../entities/ghost-clyde.hpp"
#include "../entities/ghost-slow-zone.hpp"

#include "../entities/hud.hpp"
#include "../entities/pacman.h"
#include "../entities/level.h"

using namespace suprengine;

namespace demo_pacman
{
	class GameScene : public Scene
	{
	public:
		GameScene() {}

		void init() override 
		{
			printf( "\n" );

			//  create entities & components
			auto level = new Level();

			auto slow_zone_l = new GhostSlowZone(
				Rect {
					0.0f,
					14.0f,
					6.0f,
					1.0f,
				} * Level::TILE_SIZE
			);

			auto slow_zone_r = new GhostSlowZone(
				Rect {
					22.0f,
					14.0f,
					6.0f,
					1.0f,
				} * Level::TILE_SIZE
			);

			auto pacman = new PacMan( level );
			pacman->mover->set_pos( { 14.0f, 23.0f } );

			auto manager = new GameManager();
			manager->pacman = pacman;

			auto blinky = new GhostBlinky( level, pacman );
			blinky->mover->set_pos( { 14.0f, 11.0f } );
			blinky->mover->set_state( GameManager::get_current_state() );

			auto pinky = new GhostPinky( level, pacman );
			pinky->mover->set_pos( level->get_wait_pos( pinky->mover->wait_id ) );
			pinky->mover->set_state( GhostState::WAIT );

			auto inky = new GhostInky( level, blinky, pacman );
			inky->mover->set_pos( level->get_wait_pos( inky->mover->wait_id ) );
			inky->mover->set_state( GhostState::WAIT );

			auto clyde = new GhostClyde( level, pacman );
			clyde->mover->set_pos( level->get_wait_pos( clyde->mover->wait_id ) );
			clyde->mover->set_state( GhostState::WAIT );

			//  configure camera
			auto camera_owner = new Entity();
			auto camera = new Camera( camera_owner );
			float level_width = (float) level->get_width() * Level::TILE_SIZE, level_height = (float) level->get_height() * Level::TILE_SIZE;
			camera->zoom = floorf( ( camera->viewport.h - 100.0f ) / level_height );
			camera->transform->location.x = camera->viewport.w / camera->zoom / 2.0f - level_width / 2.0f;
			camera->transform->location.y = camera->viewport.h / camera->zoom / 2.0f - level_height / 2.0f;
			camera->clip_enabled = true;
			camera->clip = {
				{ camera->viewport.x, 0.0f },
				{ level_width, camera->viewport.h }
			};

			auto hud = new HUD();
		}
	};
}