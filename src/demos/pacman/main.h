#pragma once

#include "scenes/game_scene.hpp"

void run()
{
	auto& game = Game::instance();
	if ( !game.initialize( "pac-man", 844, 844 ) ) return;

	//  set assets path
	Assets::set_path( "src/demos/pacman/assets/" );

	//  load scene
	game.set_scene( new GameScene() );

	//  start the game
	game.loop();
}