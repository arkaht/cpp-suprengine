#pragma once

#include <suprengine/scene.h>
#include <suprengine/ecs/components/box-collider.hpp>
#include <suprengine/ecs/components/mover.hpp>
#include <suprengine/ecs/components/mouse-looker.hpp>
#include <suprengine/ecs/components/renderers/mesh-renderer.hpp>

using namespace suprengine;

namespace puzzle 
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
			ground->create_component<MeshRenderer>( mesh_cube );
			ground->create_component<BoxCollider>( Box::HALF );

			//  setup cube
			auto cube = new Entity();
			cube->transform->location = Vec3 { 0.0f, 5.0f, -2.5f };
			cube->create_component<MeshRenderer>( mesh_cube );
			
			//  setup camera
			auto camera_owner = new Entity();
			camera_owner->transform->location = Vec3 { 2.0f, 7.0f, 0.0f };
			camera_owner->transform->look_at( cube->transform->location );
			camera_owner->create_component<Mover>();
			camera_owner->create_component<MouseLooker>( 2.0f );
			auto camera = camera_owner->create_component<Camera>( 77.7f );
			camera->activate();
		}
	};
}