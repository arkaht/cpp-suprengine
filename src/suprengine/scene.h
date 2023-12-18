#pragma once

namespace suprengine
{
	class IGame;
	class Engine;

	class Scene
	{
	public:
		Scene();
		virtual ~Scene() {}

		virtual void init() {}
		virtual void update( float dt ) {}

		IGame* get_game() const { return _game; }
	
	protected:
		IGame* _game { nullptr };
	};
}