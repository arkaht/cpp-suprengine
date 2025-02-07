#pragma once

#include <suprengine/components/transform.h>

#include <suprengine/utils/shareable.h>

#include <suprengine/tools/memory-profiler.h>

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
		
		/*
		 * Create and add a component of given type on the entity.
		 * This is the function you want to use inside Entity::setup.
		 */
		template <typename ComponentType, typename ...Args>
		std::enable_if_t<
			std::is_base_of_v<Component, ComponentType>,
			SharedPtr<ComponentType>
		> create_component( Args&& ...args )
		{
			SharedPtr<T> component( new ComponentType( args... ) );
			component->init( shared_from_this() );
			add_component( component );
			
			return component;
		}
		void add_component( SharedPtr<Component> component );
		void remove_component( SharedPtr<Component> component );
		/*
		 * Find for a component matching the given type from the
		 * vector of components of the entity.
		 */
		template <typename T>
		std::enable_if_t<
			std::is_base_of_v<Component, T>,
			SharedPtr<T>
		> find_component()
		{
			for ( SharedPtr<Component> component : components )
			{
				SharedPtr<T> target = std::dynamic_pointer_cast<T>( component );
				if ( !target ) continue;

				return target;
			}

			return nullptr;
		}

		/*
		 * Initialize the entity with default components such as a
		 * Transform.
		 */
		void init();
		/*
		 * Update both the entity components and itself for a frame.
		 */
		void update( float dt );
		/*
		 * Set entity's state to EntityState::Invalid. 
		 * Depending on update order, the entity will be deleted
		 * at the end of, either, the current frame or the next one.
		 */
		void kill();

		/*
		 * Called after the entity is initialized and before it's
		 * added to the engine.
		 * You should create your components here.
		 */
		virtual void setup() {};
		/*
		 * Called when the entity is updated for this frame.
		 * You can put your own logic here.
		 */
		virtual void update_this( float dt ) {}
		/*
		 * Called when debug mode is enabled at the end of the 
		 * rendering frame.
		 */
		virtual void debug_render( RenderBatch* _render_batch ) {}

		/*
		 * Get the entity's unique identifier.
		 */
		int get_unique_id() const { return _unique_id; }

	public:
		static void* operator new( std::size_t bytes )
		{
			return MemoryProfiler::allocate( "Entity", bytes );
		}

	public:
		/*
		 * Entity layer, only used by the engine for collision 
		 * detection optimization.
		 */
		uint32_t layer = 0x1;
		/*
		 * Entity state, indicating if the entity is either active,
		 * paused or queued to be destroyed.
		 */
		EntityState state { EntityState::Active };

		/*
		 * Vector of all entity components
		 */
		std::vector<SharedPtr<Component>> components;

		/*
		 * The transform component of the entity, storing location,
		 * scale and rotation.
		 */
		SharedPtr<Transform> transform;

	private:
		int _unique_id = -1;
		static int _global_id;
	};
}

