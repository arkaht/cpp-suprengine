#include <suprengine/opengl/opengl-render-batch.h>

#include "scenes/game-scene.hpp"

int main( int arg_count, char** args )
{
    auto& game = Game::instance();
	if ( !game.initialize<OpenGLRenderBatch>( "Suprkraft", 1280, 720 ) ) return EXIT_FAILURE;

	//  set assets path
	Assets::set_path( "../../../" );

	//  load scene
	game.set_scene( new puzzle::GameScene() );

	//  start the game
	game.loop();

	return EXIT_SUCCESS;
}