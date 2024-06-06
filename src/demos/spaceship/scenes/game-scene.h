#pragma once

#include <suprengine/scene.h>
#include <suprengine/components/colliders/box-collider.h>
#include <suprengine/components/mover.hpp>
#include <suprengine/components/mouse-looker.hpp>
#include <suprengine/random.h>

#include <entities/player-spaceship-controller.h>
#include <entities/ai-spaceship-controller.h>
#include <entities/asteroid.h>

namespace spaceship 
{
	class GameInstance;

	class GameScene : public Scene
	{
	public:
		GameScene( GameInstance* game_instance );

		void init() override;
		void update( float dt ) override;

		void generate_ai_spaceships( int count );

	private:
		shared_ptr<Spaceship> spaceship1;
		shared_ptr<Spaceship> spaceship2;

		GameInstance* _game_instance { nullptr };

		shared_ptr<PlayerSpaceshipController> player_controller;
		shared_ptr<AISpaceshipController> ai_controller;

		float spawn_time { 0.0f };

		unsigned int _seed = (unsigned int)std::time( nullptr );

		shared_ptr<Camera> _temporary_camera;
		shared_ptr<ModelRenderer> _temporary_camera_model;

		Vec3 _player_location = Vec3::zero;
		Quaternion _player_rotation = Quaternion::identity;
	};
}