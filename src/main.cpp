#include "SDL.h"
#include "game.h"

#include "suprengine/ecs/entity.h"
#include "suprengine/ecs/components/rect_renderer.hpp"


const int SCREEN_WIDTH = 1280, SCREEN_HEIGHT = 720;

int main( int arg_count, char** args )
{
	//  create game
	auto& game = Game::instance();
	if ( !game.initialize() )
	{
		return 0;
	}

	//  prepare some entities
	Entity* my_ent = new Entity();
	new RectRenderer( my_ent, Rect { -64.0f, -64.0f, 128.0f, 128.0f } );

	//  game loop
	game.loop();

	return 0;
}