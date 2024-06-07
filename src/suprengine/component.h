#pragma once

#include "component.fwd.h"
#include "entity.fwd.h"
#include "components/transform.fwd.h"

#include <suprengine/render-batch.h>
#include <suprengine/shareable.hpp>

namespace suprengine
{
	class Component : public Shareable<Component>
	{
	public:
		Component();
		Component( int priority_order );
		virtual ~Component() {};

		Component( const Component& ) = delete;
		Component& operator=( const Component& ) = delete;

		void init( SharedPtr<Entity> owner );

		virtual void setup() {}
		virtual void unsetup() {}

		virtual void update( float dt ) {}
		virtual void debug_render( RenderBatch* _render_batch ) {}

		SharedPtr<Entity> get_owner() const;
		int get_priority_order() const;
		
	public:
		SharedPtr<Transform> transform;

		bool is_active { true };

	private:
		int _priority_order;

		/*
		 * Weak reference to the entity owner.
		 * It must be a weak pointer because a shared pointer would
		 * count for the ownership of that entity, preventing its
		 * deletion inside the memory.
		 */
		WeakPtr<Entity> _owner;
	};
}