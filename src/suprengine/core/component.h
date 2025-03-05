#pragma once

#include <suprengine/core/render-batch.h>

#include <suprengine/utils/shareable.h>

#include <suprengine/tools/memory-profiler.h>

namespace suprengine
{
	class Entity;
	class Transform;

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
	#ifdef ENABLE_MEMORY_PROFILER
		static void* operator new( std::size_t bytes )
		{
			return MemoryProfiler::allocate( "Component", bytes );
		}
	#endif
		
	public:
		/*
		 * Component handling the position, rotation and scale
		 * of the entity.
		 */
		SharedPtr<Transform> transform = nullptr;

		/*
		 * Control whenever the component is enabled in order
		 * for the engine to update and render it in-game.
		 */
		bool is_active = true;

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