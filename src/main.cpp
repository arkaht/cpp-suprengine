#include "suprengine/game.h"

#include <suprengine/ecs/entity.h>
#include <suprengine/ecs/components/renderers/sprite_renderer.hpp>

using namespace suprengine;

int main( int arg_count, char** args )
{
	//  create game
	auto& game = Game::instance();
	if ( !game.initialize( "my-engine-demo", 1280, 720 ) )
	{
		return 0;
	}

	//  load assets
	Assets::set_render_batch( game.get_render_batch() );

	//  prepare some entities
	Entity* my_ent = new Entity();
	//new RectRenderer( my_ent, Rect { -64.0f, -64.0f, 128.0f, 128.0f }, Color::blue );
	auto sprite = new SpriteRenderer( my_ent, Assets::get_texture( "atlas.png" ) );
	//my_ent->transform->rotation = 90.0f;

	//  game loop
	game.loop();

	return 0;
}