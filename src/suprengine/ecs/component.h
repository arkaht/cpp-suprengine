#pragma once

#include "component.fwd.h"
#include "entity.fwd.h"

#include <suprengine/render-batch.h>

namespace suprengine
{
	class Component
	{
	protected:
		Entity* owner;
		int priority_order;
	public:
		bool is_updated { true };

		Component( Entity* owner, int priority_order = 0 );
		virtual ~Component();

		Component() = delete;
		Component( const Component& ) = delete;
		Component& operator=( const Component& ) = delete;

		virtual void update( float dt );
		virtual void debug_render( RenderBatch* render_batch ) {}

		Entity* get_owner() const { return owner; }
		int get_priority_order() const { return priority_order; }
	};
}