#pragma once

#include <suprengine/core/game.h>
#include <suprengine/core/physics.h>
#include <suprengine/core/scene.h>
#include <suprengine/core/updater.h>
#include <suprengine/input/input-manager.h>

#include <suprengine/components/camera.h>

#include <suprengine/utils/assert.h>
#include <suprengine/utils/timer.h>

#include <suprengine/tools/memory-profiler.h>
#include <suprengine/tools/profiler.h>

#include <vector>

namespace suprengine
{
	class Engine
	{
	public:
		/*
		 * Engine singleton.
		 */
		static Engine& instance()
		{
			static Engine instance {};
			return instance;
		}

		Engine( const Engine& ) = delete;
		Engine( Engine&& ) = delete;
		Engine& operator=( const Engine& ) = delete;
		Engine& operator=( Engine&& ) = delete;
		~Engine();

		/*
		 * Create and run the game of given type on the engine.
		 */
		template <typename GameType>
		std::enable_if_t<
			std::is_base_of_v<IGame, GameType>,
			int
		> run()
		{
			if ( !init<GameType>() ) return EXIT_FAILURE;

			loop();
			return EXIT_SUCCESS;
		}

		template <typename GameType>
		std::enable_if_t<
			std::is_base_of_v<IGame, GameType>,
			bool
		> init()
		{
			MEMORY_SCOPE( "Engine::init" );

			//  Create game
			auto game = new GameType();
			return init( game );
		}
		bool init( IGame* game );
		void loop();

		template <typename SceneType, typename ...Args>
		std::enable_if_t<
			std::is_base_of_v<Scene, SceneType>,
			SceneType*
		> create_scene( Args&& ...args )
		{
			clear_entities();

			SceneType* new_scene = new SceneType( args... );
			
			//  Change scene
			_scene = std::unique_ptr<Scene>( new_scene );
			_scene->init();

			return new_scene;
		}
		Scene* get_scene() const { return _scene.get(); }

		template <typename EntityType, typename ...Args>
		std::enable_if_t<
			std::is_base_of_v<Entity, EntityType>,
			SharedPtr<EntityType>
		> create_entity( Args&& ...args )
		{
			SharedPtr<EntityType> entity( new EntityType( args... ) );
			entity->init();
			entity->setup();
			add_entity( entity );

			return entity;
		}
		void add_entity( SharedPtr<Entity> entity );
		void remove_entity( SharedPtr<Entity> entity );
		void clear_entities();

		void add_timer( const Timer& timer );

		bool is_running() const;

		IGame* get_game() const { return _game.get(); }
		Window* get_window() const { return _window.get(); }
		RenderBatch* get_render_batch() const { return _render_batch.get(); }
		InputManager* get_inputs() const { return _inputs.get(); }
		Physics* get_physics() const { return _physics.get(); }
		Updater* get_updater() { return &_updater; }
		Profiler* get_profiler() { return &_profiler; }

	public:
		/*
		 * Called when an entity is being added.
		 * 
		 * Parameters:
		 * - SafePtr<Entity> Entity being added
		 */
		Event<SafePtr<Entity>> on_entity_added {};
		/*
		 * Called when an entity is being removed.
		 * 
		 * Parameters:
		 * - Entity* Entity being removed
		 */
		Event<Entity*> on_entity_removed {};
		/*
		 * Called when ImGui windows are updating.
		 */
		Event<> on_imgui_update {};

	public:
		Camera* camera { nullptr };
		bool is_game_paused = false;

	private:
		bool _is_running = true;
		bool _is_updating = false;

	private:
		Engine() = default;

		void process_input();
		void update( float dt );
		void render();

	private:
		std::vector<SharedPtr<Entity>> _pending_entities {}, _entities {}, _dead_entities {};

		std::vector<Timer> _timers {};

		std::unique_ptr<IGame> _game;
		std::unique_ptr<Window> _window;
		std::unique_ptr<RenderBatch> _render_batch;
		std::unique_ptr<InputManager> _inputs;
		std::unique_ptr<Physics> _physics;

		Profiler _profiler {};
		Updater _updater {};

		std::unique_ptr<Scene> _scene;
	};
}
