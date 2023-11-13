#pragma once
#include "scene.fwd.h"

#include "game.fwd.h"

namespace suprengine
{
	class Scene
	{
	public:
		Scene();
		virtual ~Scene() {}

		virtual void init() {}
		virtual void update( float dt ) {}

		Game* get_game() const { return _game; }
	
	protected:
		Game* _game { nullptr };
	};
}