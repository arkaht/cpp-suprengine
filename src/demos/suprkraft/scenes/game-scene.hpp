#pragma once

#include <suprengine/scene.h>
#include <suprengine/ecs/components/box-collider.hpp>
#include <suprengine/ecs/components/mover.hpp>
#include <suprengine/ecs/components/mouse-looker.hpp>
#include <suprengine/ecs/components/renderers/mesh-renderer.hpp>

#include <demos/opengl-3d/camera-demo.hpp>

using namespace suprengine;

namespace suprkraft 
{
	class GameScene : public Scene
	{
	public:
		GameScene() {}

		void init() override 
		{
			game->get_inputs()->set_relative_mouse_mode( true );
			game->get_render_batch()->set_background_color( Color::from_0x( 0x252627FF ) );

			Mesh* mesh_cube = Assets::get_mesh( Assets::PRIMITIVE_CUBE_PATH, false );

			//  setup ground
			auto ground = new Entity();
			ground->transform->scale = Vec3 { 100.0f, 1.0f, 100.0f };
			new MeshRenderer( ground, mesh_cube );
			new BoxCollider( ground,
				Box {
					Vec3 { -0.5f, -0.5f, -0.5f },
					Vec3 { 0.5f, 0.5f, 0.5f }
				}
			);

			//  setup cube
			auto cube = new Entity();
			cube->transform->location = Vec3 { 0.0f, 5.0f, -2.5f };
			new MeshRenderer( cube, mesh_cube );
			/*new BoxCollider( cube,
				Box {
					Vec3 { -0.5f, -0.5f, -0.5f },
					Vec3 { 0.5f, 0.5f, 0.5f }
				}
			);*/

			//  setup camera
			auto camera_owner = new Entity();
			camera_owner->transform->location = Vec3 { 2.0f, 7.0f, 0.0f };
			camera_owner->transform->look_at( cube->transform->location );
			new Mover( camera_owner );
			new MouseLooker( camera_owner, 2.0f );
			auto camera = new Camera( camera_owner, 77.7f );
			camera->activate();
		}
	};
}