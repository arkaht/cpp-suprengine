#pragma once

#include "entity.fwd.h"
#include "component.fwd.h"
#include "components/collider.fwd.h"
#include "components/transform.h"

#include <suprengine/engine.h>
#include <suprengine/mtx4.h>

#include <vector>

namespace suprengine
{
	enum class EntityState
	{
		Active,
		Paused,
		Invalid,
	};

	class Entity
	{
	public:
		uint32_t layer = 0x1;
		EntityState state { EntityState::Active };

		std::vector<std::shared_ptr<Component>> components;

		std::shared_ptr<Transform> transform;
		std::shared_ptr<Collider> collider { nullptr };

		Entity();
		virtual ~Entity();
		Entity( const Entity& ) = delete;
		Entity& operator=( const Entity& ) = delete;
		
		template <typename T, typename... Args>
		std::shared_ptr<T> create_component( Args&&... args )
		{
			static_assert( std::is_base_of<Component, T>::value, "Entity::create_component: used for a non-Component class!" );
			
			auto component = std::make_shared<T>( this, args... );
			component->setup();
			add_component( component );
			
			return component;
		}
		void add_component( std::shared_ptr<Component> component );
		void remove_component( std::shared_ptr<Component> component );

		void update( float dt );
		void kill();

		virtual void update_this( float dt ) {}
		virtual void debug_render( RenderBatch* _render_batch ) {};

		virtual void on_trigger_enter( std::shared_ptr<Collider> collider ) {};
		virtual void on_trigger_stay( std::shared_ptr<Collider> collider ) {};
		virtual void on_trigger_exit( std::shared_ptr<Collider> collider ) {};

		Engine* get_game() const { return _engine; }
		int get_unique_id() const { return _unique_id; }

	protected:
		Engine* _engine { nullptr };

	private:
		int _unique_id = -1;
		static int _global_id;
	};
}

