#pragma once

#include <suprengine/scene.h>
#include <suprengine/ecs/components/mouse-follower.hpp>
#include <suprengine/ecs/components/renderers/rect-renderer.hpp>
#include <suprengine/ecs/components/renderers/sprite-renderer.hpp>
#include <suprengine/ecs/components/renderers/text-renderer.hpp>
#include <suprengine/ecs/components/renderers/mesh-renderer.hpp>

using namespace suprengine;

namespace demo_opengl3d
{
	class TimeRotator : Component
	{
	public:
		TimeRotator( Entity* owner ) : Component( owner ) {}

		void update( float dt ) override
		{
			Quaternion increment = Quaternion( Vec3::forward, dt  );
			Quaternion rotation = Quaternion::concatenate( owner->transform->rotation, increment );
			owner->transform->set_rotation( rotation );
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

			auto cube = new Entity();
			cube->transform->location = Vec3 { 0.0f, 0.0f, 10.0f };
			cube->transform->rotation = Quaternion( Vec3 { 45.0f, 45.0f, 45.0f } );
			new MeshRenderer( cube, Assets::get_mesh( "src/suprengine/assets/meshes/primitives/cube.gpmesh", false ) );
			new TimeRotator( cube );

			auto sphere = new Entity();
			sphere->transform->location = Vec3 { 0.0f, 10.0f, 20.0f };
			new MeshRenderer( sphere, Assets::get_mesh( "src/suprengine/assets/meshes/primitives/sphere.gpmesh", false ) );
			/*new MouseFollower( ent );
			new TimeRotator( ent );

			( new TextRenderer( ent, Assets::get_font( "PressStart2P.ttf" ), "Hello!" ) )->modulate = Color::green;
			( new RectRenderer( ent, Rect { 0.0f, 0.0f, 256.0f, 128.0f } ) )->modulate = Color::from_0x( 0xFF000022 );
			( new RectRenderer( ent, Rect { 1024.0f, 512.0f, 128.0f, 512.0f } ) )->modulate = Color::green;
			( new RectRenderer( ent, Rect { 512.0f, 128.0f, 128.0f, 256.0f } ) )->modulate = Color::blue;*/

			//auto sprite = new SpriteRenderer( ent, Assets::get_texture( "level.png" ) );
			//sprite->origin = Vec2::one;
			//sprite->dest = { 0.0f, 0.0f, 512.0f, 512.0f };
			//sprite->modulate = Color::from_0x( 0xBFB48FFF );

			auto camera = game->get_camera();
			//camera->setup_perspective( 90.0f );
			camera->setup_perspective( 77.7f );
			camera->look_at( cube->transform->location );
			//camera->translate( -camera->viewport.get_size() );  //  NOTE: wrong opengl renderer offset
			//camera->translate( Vec2 { 0, 0 } );
			//camera->zoom = 2.0f;
		}
	};
}