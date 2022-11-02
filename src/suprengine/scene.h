#pragma once
#include "scene.fwd.h"

#include "game.h"
#include "ecs/entity.h"

namespace suprengine
{
	class Scene
	{
	protected:
		Game* game { &Game::instance() };
	public:
		Scene() {}
		virtual ~Scene() {}

		Game* get_game() const { return game; }
	};
}