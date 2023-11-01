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
		bool is_initialized { false };
		bool should_update { true };

		std::shared_ptr<Transform> transform;

		Component( Entity* owner, int priority_order = 0 );
		virtual ~Component() {};

		Component() = delete;
		Component( const Component& ) = delete;
		Component& operator=( const Component& ) = delete;

		template <typename T>
		std::shared_ptr<T> get_shared_from_this()
		{
			return std::static_pointer_cast<T>( shared_from_this() );
		}

		virtual void setup() {}
		virtual void unsetup() {}

		virtual void init() {}
		virtual void update( float dt ) {}
		virtual void debug_render( RenderBatch* render_batch ) {}

		Entity* get_owner() const { return owner; }
		int get_priority_order() const { return priority_order; }
	
	protected:
		Entity* owner { nullptr };
		int priority_order;
	};
}