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

	printf( "%i; %i\n", level->get_width(), level->get_height() );
	auto camera = game.get_camera();
	float level_width = level->get_width() * Level::TILE_SIZE, level_height = level->get_height() * Level::TILE_SIZE;
	camera->translate(
		Vec2 {
			camera->viewport.w / 2.0f - level_width / 2.0f,
			camera->viewport.h / 2.0f - level_height / 2.0f
		}
	);
	camera->zoom = ( camera->viewport.h /*- 50.0f*/ ) / level_height;
	printf( "z%f; %f; %f; %f; %f\n", camera->zoom, camera->viewport.x, camera->viewport.y, camera->viewport.w, camera->viewport.h );

	/*Entity* ghost_frightened_anim = new Entity();
	ghost_frightened_anim->transform->pos = { 160.0f + 48.0f, 160.0f + 24.0f };
	auto anim_sprite = new AnimSpriteRenderer( ghost_frightened_anim, Assets::get_texture( "atlas.png" ), 6, Color::green );
	anim_sprite->gen_frames( Rect { 128.0f, 16.0f, 64.0f, 16.0f }, Vec2 { 16.0f, 16.0f } );
	anim_sprite->dest = { 0.0f, 0.0f, 16.0f, 16.0f };
	anim_sprite->set_fps( 16 );*/

	//  start the game
	game.loop();
}