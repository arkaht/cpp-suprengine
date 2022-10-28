#pragma once

#include "entity.fwd.h"
#include "component.fwd.h"
#include "components/transform2.fwd.h"
#include "suprengine/game.h"

#include <vector>

namespace suprengine
{
	enum class EntityState
	{
		ACTIVE,
		PAUSED,
		DEAD,
	};

	class Entity
	{
	private:
		EntityState _state { EntityState::ACTIVE };
		Game* _game { &Game::instance() };
	public:
		std::vector<Component*> components;
		Transform2* transform;

		Entity();
		~Entity();
		Entity( const Entity& ) = delete;
		Entity& operator=( const Entity& ) = delete;

		void add_component( Component* comp );
		void remove_component( Component* comp );
		std::vector<Component*>::iterator get_component_iterator( Component* comp );

		void update( float dt );
		void kill();

		Game* get_game() const { return _game; }
		EntityState get_state() const { return _state; }
	};
}

