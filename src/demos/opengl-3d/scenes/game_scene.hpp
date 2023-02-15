#pragma once

#include <suprengine/scene.h>
#include <suprengine/ecs/components/mouse-follower.hpp>
#include <suprengine/ecs/components/renderers/rect_renderer.hpp>
#include <suprengine/ecs/components/renderers/sprite_renderer.hpp>

using namespace suprengine;

namespace demo_opengl3d
{
	class GameScene : public Scene
	{
	public:
		GameScene() {}

		void init() override 
		{
			printf( "\n" );

			game->get_render_batch()->set_background_color( Color::from_0x( 0x252627FF ) );

			auto ent = new Entity();
			new MouseFollower( ent );

			( new RectRenderer( ent, Rect { 0.0f, 0.0f, 256.0f, 128.0f } ) )->modulate = Color::from_0x( 0xFF000022 );
			( new RectRenderer( ent, Rect { 1024.0f, 512.0f, 128.0f, 512.0f } ) )->modulate = Color::green;
			( new RectRenderer( ent, Rect { 512.0f, 128.0f, 128.0f, 256.0f } ) )->modulate = Color::blue;

			auto sprite = new SpriteRenderer( ent, Assets::get_texture( "level.png" ) );
			//sprite->dest = { 0.0f, 0.0f, 512.0f, 512.0f };
			sprite->modulate = Color::from_0x( 0xBFB48FFF );
		}
	};
}