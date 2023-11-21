#pragma once

#include <suprengine/scene.h>
#include <suprengine/components/colliders/box-collider.hpp>
#include <suprengine/components/mover.hpp>
#include <suprengine/components/mouse-looker.hpp>

#include <components/stylized-model-renderer.hpp>
#include <entities/player-spaceship.h>

namespace puzzle 
{
	class GameScene : public Scene
	{
	public:
		void init() override 
		{
			load_assets();
			init_game();

			//  setup planet
			auto planet = new Entity();
			planet->transform->location = Vec3 { 150.0f, 100.0f, 0.0f };
			planet->transform->rotation = Quaternion( Vec3 { 0.0f, 12.0f, -6.0f } );
			planet->transform->scale = Vec3( 1.0f );
			planet->create_component<StylizedModelRenderer>( 
				Assets::get_model( "planet-ring" ), 
				Color::from_0x( 0x1c6cF0FF )
			);
			
			//  setup cube
			spaceship = new PlayerSpaceship();
			spaceship->transform->location = Vec3 { 0.0f, 0.0f, 0.0f };
		}
		
		void update( float dt ) override {}

	private:
		void load_assets()
		{
			//  shaders
			Assets::load_shader(
				"stylized",
				"assets/puzzle/shaders/stylized.vert",
				"assets/puzzle/shaders/stylized.frag"
			);

			//  models
			Assets::load_model( "spaceship", "assets/puzzle/models/spaceship2.fbx" );
			Assets::load_model( "projectile", "assets/puzzle/models/projectile.fbx" );
			Assets::load_model( "planet-ring", "assets/puzzle/models/planet-ring.fbx" );
		}

		void init_game()
		{
			//_game->get_inputs()->set_relative_mouse_mode( true );

			RenderBatch* render_batch = _game->get_render_batch();
			render_batch->set_background_color( Color::from_0x( 0x00000000 ) );
		}

		std::shared_ptr<Camera> camera;
		Entity* spaceship;
	};
}