#pragma once

#include "component.fwd.h"
#include "entity.fwd.h"

namespace suprengine
{
	class Component
	{
	protected:
		Entity* owner;
		int priority_order;
	public:
		Component( Entity* owner, int priority_order = 0 );
		virtual ~Component();

		Component() = delete;
		Component( const Component& ) = delete;
		Component& operator=( const Component& ) = delete;

		virtual void update( float dt );

		int get_priority_order() const { return priority_order; }
	};
}