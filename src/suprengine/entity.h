#pragma once

#include "entity.fwd.h"
#include "component.fwd.h"
#include "components/collider.fwd.h"
#include "components/transform.h"

#include <suprengine/engine.h>
#include <suprengine/shareable.hpp>

#include <vector>

namespace suprengine
{
	enum class EntityState
	{
		Active,
		Paused,
		Invalid,
	};

	class Entity : public Shareable<Entity>
	{
	public:
		Entity();
		virtual ~Entity();
		Entity( const Entity& ) = delete;
		Entity& operator=( const Entity& ) = delete;
		
		template <typename T, typename... Args>
		shared_ptr<T> create_component( Args&&... args )
		{
			static_assert( std::is_base_of<Component, T>::value, "Entity::create_component: used for a non-Component class!" );
			
			auto component = std::make_shared<T>( args... );
			component->init( shared_from_this() );
			add_component( component );
			
			return component;
		}
		//  TODO: Remove
		template <typename T, typename... Args>
		shared_ptr<T> create_component_pro( Args&&... args )
		{
			static_assert( std::is_base_of<Component, T>::value, "Entity::create_component: used for a non-Component class!" );
			
			auto component = std::make_shared<T>( args... );
			component->init( shared_from_this() );
			add_component( component );
			
			return component;
		}
		void add_component( shared_ptr<Component> component );
		void remove_component( shared_ptr<Component> component );
		template <typename T>
		shared_ptr<T> get_component()
		{
			static_assert( std::is_base_of<Component, T>::value, "Entity::get_component: used for a non-Component class!" );
			
			for ( shared_ptr<Component> component : components )
			{
				shared_ptr<T> target = std::dynamic_pointer_cast<T>( component );
				if ( !target ) continue;

				return target;
			}

			return nullptr;
		}

		void init();
		void update( float dt );
		void kill();

		/*
		 * Called after the entity is initialized and before it's
		 * added to the engine.
		 * You should create your components here.
		 */
		virtual void setup() {};
		virtual void update_this( float dt ) {}
		virtual void debug_render( RenderBatch* _render_batch ) {}

		int get_unique_id() const { return _unique_id; }

	public:
		uint32_t layer = 0x1;
		EntityState state { EntityState::Active };

		std::vector<shared_ptr<Component>> components;

		shared_ptr<Transform> transform;

	private:
		int _unique_id = -1;
		static int _global_id;
	};
}

