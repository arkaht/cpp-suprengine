#pragma once

#include "component.fwd.h"
#include "entity.fwd.h"
#include "components/transform.fwd.h"

#include <suprengine/render-batch.h>

namespace suprengine
{
	class Component : public std::enable_shared_from_this<Component>
	{
	public:
		Component();
		Component( int priority_order );
		virtual ~Component() {};

		Component( const Component& ) = delete;
		Component& operator=( const Component& ) = delete;

		template <typename T>
		std::shared_ptr<T> get_shared_from_this()
		{
			return std::static_pointer_cast<T>( shared_from_this() );
		}

		void init( shared_ptr<Entity> owner );

		virtual void setup() {}
		virtual void unsetup() {}

		virtual void update( float dt ) {}
		virtual void debug_render( RenderBatch* _render_batch ) {}

		shared_ptr<Entity> get_owner() const;
		int get_priority_order() const;
		
	public:
		shared_ptr<Transform> transform;

		bool is_active { true };

	private:
		int _priority_order;

		/*
		 * Weak reference to the entity owner.
		 * It must be a weak pointer because a shared pointer would
		 * count for the ownership of that entity, preventing its
		 * deletion inside the memory.
		 */
		weak_ptr<Entity> _owner;
	};
}