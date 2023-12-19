#pragma once

#include <suprengine/scene.h>
#include <suprengine/components/colliders/box-collider.h>
#include <suprengine/components/mover.hpp>
#include <suprengine/components/mouse-looker.hpp>
#include <suprengine/random.h>

#include <entities/player-spaceship-controller.h>
#include <entities/asteroid.h>

namespace puzzle 
{
	class GameScene : public Scene
	{
	public:
		void init() override;
		void update( float dt ) override;

	private:
		Spaceship* spaceship1 { nullptr };
		Spaceship* spaceship2 { nullptr };
		PlayerSpaceshipController* player_controller { nullptr };
	};
}