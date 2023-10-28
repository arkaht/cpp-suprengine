#pragma once
#include "scene.fwd.h"

#include "game.h"
#include "ecs/entity.h"

namespace suprengine
{
	class Scene
	{
	public:
		Scene() {}
		virtual ~Scene() {}

		virtual void init() {};

		Game* get_game() const { return game; }
	
	protected:
		Game* game { &Game::instance() };
	};
}