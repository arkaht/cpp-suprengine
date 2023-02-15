#pragma once

#include "entity.fwd.h"
#include "component.fwd.h"
#include "components/collider.fwd.h"
#include "components/transform2.fwd.h"

#include <suprengine/game.h>
#include <suprengine/mtx4.h>

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
	protected:
		EntityState state { EntityState::ACTIVE };
		Game* game { &Game::instance() };
	public:
		uint32_t layer = 0x1;

		std::vector<Component*> components;

		Transform2* transform { nullptr };
		Collider* collider { nullptr };

		Entity();
		~Entity();
		Entity( const Entity& ) = delete;
		Entity& operator=( const Entity& ) = delete;

		void add_component( Component* comp );
		void remove_component( Component* comp );
		std::vector<Component*>::iterator get_component_iterator( Component* comp );

		void update( float dt );
		void kill();

		virtual void update_this( float dt ) {}
		virtual void debug_render( RenderBatch* render_batch ) {};

		virtual void on_trigger_enter( Collider* collider ) {};
		virtual void on_trigger_stay( Collider* collider ) {};
		virtual void on_trigger_exit( Collider* collider ) {};

		Game* get_game() const { return game; }

		void set_state( EntityState state );
		EntityState get_state() const { return state; }
	};
}

