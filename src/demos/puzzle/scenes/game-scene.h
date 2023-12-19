#pragma once

#include <suprengine/scene.h>
#include <suprengine/components/colliders/box-collider.h>
#include <suprengine/components/mover.hpp>
#include <suprengine/components/mouse-looker.hpp>
#include <suprengine/random.h>

#include <components/stylized-model-renderer.hpp>
#include <entities/player-spaceship.h>
#include <entities/asteroid.h>

namespace puzzle 
{
	class GameScene : public Scene
	{
	public:
		void init() override;
		void update( float dt ) override;

	private:
		Entity* spaceship;
	};
}