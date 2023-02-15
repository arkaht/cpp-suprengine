#pragma once

#include <suprengine/scene.h>
#include <suprengine/ecs/components/mouse-follower.hpp>
#include <suprengine/ecs/components/renderers/rect_renderer.hpp>
#include <suprengine/ecs/components/renderers/sprite_renderer.hpp>
#include <suprengine/ecs/components/renderers/text_renderer.hpp>

using namespace suprengine;

namespace demo_opengl3d
{
	class TimeRotator : Component
	{
	public:
		TimeRotator( Entity* owner ) : Component( owner ) {}

		void update( float dt ) override
		{
			owner->transform->rotation += dt * 50.0f;
		}
	};

	class GameScene : public Scene
	{
	public:
		GameScene() {}

		void init() override 
		{
			printf( "\n" );

			game->get_render_batch()->set_background_color( Color::from_0x( 0x252627FF ) );

			auto ent = new Entity();
			ent->transform->rotation = 20.0f;
			new MouseFollower( ent );
			new TimeRotator( ent );

			( new TextRenderer( ent, Assets::get_font( "PressStart2P.ttf" ), "Hello!" ) )->modulate = Color::green;
			( new RectRenderer( ent, Rect { 0.0f, 0.0f, 256.0f, 128.0f } ) )->modulate = Color::from_0x( 0xFF000022 );
			( new RectRenderer( ent, Rect { 1024.0f, 512.0f, 128.0f, 512.0f } ) )->modulate = Color::green;
			( new RectRenderer( ent, Rect { 512.0f, 128.0f, 128.0f, 256.0f } ) )->modulate = Color::blue;

			auto sprite = new SpriteRenderer( ent, Assets::get_texture( "level.png" ) );
			sprite->origin = Vec2::zero;
			//sprite->dest = { 0.0f, 0.0f, 512.0f, 512.0f };
			//sprite->modulate = Color::from_0x( 0xBFB48FFF );
		}
	};
}