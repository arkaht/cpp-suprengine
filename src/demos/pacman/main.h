#pragma once

#include <suprengine/game.h>

#include "entities/pacman.hpp"
#include "entities/level.hpp"

using namespace suprengine;

void run()
{
	auto& game = Game::instance();
	if ( !game.initialize( "pac-man" ) ) return;

	//  set assets path
	Assets::set_path( "src/demos/pacman/assets/" );

	//  create entities & components
	auto level = new Level();

	auto pacman = new PacMan( level );
	pacman->mover->set_pos( { 14, 23 } );

	Entity* ghost_frightened_anim = new Entity();
	ghost_frightened_anim->transform->pos = { 160.0f + 48.0f, 160.0f + 24.0f };
	auto anim_sprite = new AnimSpriteRenderer( ghost_frightened_anim, Assets::get_texture( "atlas.png" ), 6, Color::green );
	anim_sprite->gen_frames( Rect { 128.0f, 16.0f, 64.0f, 16.0f }, Vec2 { 16.0f, 16.0f } );
	anim_sprite->dest = { 0.0f, 0.0f, 16.0f, 16.0f };
	anim_sprite->set_fps( 16 );

	//  start the game
	game.loop();
}