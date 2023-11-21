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

			Model* cube_model = Assets::get_model( "suprengine::cube" );
			Model* cylinder_model = Assets::get_model( "suprengine::cylinder" );
			Model* sphere_model = Assets::get_model( "suprengine::sphere" );

			//  setup planet
			auto planet = new Entity();
			planet->transform->location = Vec3 { 100.0f, 75.0f, 50.0f };
			planet->transform->scale = Vec3( 50.0f );
			planet->create_component<StylizedModelRenderer>( sphere_model, Color::blue );
			
			//  setup cube
			spaceship = new PlayerSpaceship();
			spaceship->transform->location = Vec3 { 0.0f, 0.0f, 0.0f };
		}
		
		void update( float dt ) override {}

	private:
		void load_assets()
		{
			auto shader = Assets::load_shader(
				"stylized",
				"assets/puzzle/shaders/stylized.vert",
				"assets/puzzle/shaders/stylized.frag"
			);
			auto spaceship_model = Assets::load_model(
				"spaceship",
				"assets/puzzle/models/spaceship2.fbx",
				""
			);
			Assets::load_model( "projectile", "assets/puzzle/models/projectile.fbx", "" );
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