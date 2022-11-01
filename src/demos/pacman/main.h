#pragma once

#include <suprengine/game.h>

#include "entities/ghost_blinky.hpp"
#include "entities/ghost_pinky.hpp"
#include "entities/ghost_inky.hpp"
#include "entities/ghost_clyde.hpp"

#include "entities/pacman.hpp"
#include "entities/level.hpp"

using namespace suprengine;

void run()
{
	auto& game = Game::instance();
	if ( !game.initialize( "pac-man", 844, 844 ) ) return;

	//  set assets path
	Assets::set_path( "src/demos/pacman/assets/" );

	//  create entities & components
	auto level = new Level();

	auto pacman = new PacMan( level );
	pacman->mover->set_pos( { 14, 23 } );

	auto blinky = new GhostBlinky( level, pacman );
	blinky->mover->set_pos( { 14, 11 } );
	blinky->mover->set_state( GhostState::FLEE );

	auto pinky = new GhostPinky( level, pacman );
	pinky->mover->set_pos( { 13, 11 } );
	pinky->mover->set_state( GhostState::FLEE );

	auto inky = new GhostInky( level, blinky, pacman );
	inky->mover->set_pos( { 12, 11 } );
	inky->mover->set_state( GhostState::FLEE );

	auto clyde = new GhostClyde( level, pacman );
	clyde->mover->set_pos( { 11, 11 } );
	clyde->mover->set_state( GhostState::FLEE );

	//  configure camera
	auto camera = game.get_camera();
	float level_width = level->get_width() * Level::TILE_SIZE, level_height = level->get_height() * Level::TILE_SIZE;
	camera->zoom = (int) floorf( ( camera->viewport.h - 100.0f ) / level_height );
	camera->translate(
		Vec2 {
			camera->viewport.w / camera->zoom / 2.0f - level_width / 2.0f,
			camera->viewport.h / camera->zoom / 2.0f - level_height / 2.0f
		}
	);

	/*Entity* ghost_frightened_anim = new Entity();
	ghost_frightened_anim->transform->pos = { 160.0f + 48.0f, 160.0f + 24.0f };
	auto anim_sprite = new AnimSpriteRenderer( ghost_frightened_anim, Assets::get_texture( "atlas.png" ), 6, Color::green );
	anim_sprite->gen_frames( Rect { 128.0f, 16.0f, 64.0f, 16.0f }, Vec2 { 16.0f, 16.0f } );
	anim_sprite->dest = { 0.0f, 0.0f, 16.0f, 16.0f };
	anim_sprite->set_fps( 16 );*/

	//  start the game
	game.loop();
}