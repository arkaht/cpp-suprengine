#pragma once

#include <suprengine/opengl/opengl-render-batch.h>

#include "scenes/game-scene.hpp"

void run()
{
	auto& game = Game::instance();
	if ( !game.initialize<OpenGLRenderBatch>( "Suprkraft", 1280, 720 ) ) return;

	//  set assets path
	Assets::set_path( "src/demos/pacman/assets/" );

	//  load scene
	game.set_scene( new suprkraft::GameScene() );

	//  start the game
	game.loop();
}