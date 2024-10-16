#pragma once

#include <suprengine/updater.h>
#include <suprengine/scene.h>
#include <suprengine/timer.hpp>

#include <suprengine/input-manager.h>
#include <suprengine/physics.h>
#include <suprengine/game.h>

#include <suprengine/components/camera.h>

#include <suprengine/imgui.h>

#include <suprengine/assert.hpp>

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
			static Engine inst;
			return inst;
		}

		Engine( const Engine& ) = delete;
		Engine( Engine&& ) = delete;
		Engine& operator=( const Engine& ) = delete;
		Engine& operator=( Engine&& ) = delete;
		~Engine();

		/*
		 * Create and run the game of given type on the engine.
		 */
		template <typename TGame>
		int run()
		{
			if ( !init<TGame>() ) return EXIT_FAILURE;

			loop();
			return EXIT_SUCCESS;
		}

		template <typename TGame>
		bool init()
		{
			static_assert( 
				std::is_base_of<IGame, TGame>::value, 
				"Engine::init: use of an invalid Game class!" 
			);

			//  Create game
			auto game = new TGame();
			return init( game );
		}
		bool init( IGame* game );
		void loop();

		template <typename TScene, typename ...Args>
		void create_scene( Args&& ...args )
		{
			static_assert( 
				std::is_base_of<Scene, TScene>::value, 
				"Engine::create_scene: used for a non-Scene class!"
			);

			clear_entities();

			//  Change scene
			_scene = std::make_unique<TScene>( args... );
			_scene->init();
		}
		Scene* get_scene() const { return _scene.get(); }

		template <typename TEntity, typename ...Args>
		SharedPtr<TEntity> create_entity( Args&& ...args )
		{
			static_assert( 
				std::is_base_of<Entity, TEntity>::value, 
				"Engine::create_entity: used for a non-Entity class!"
			);

			auto entity = std::make_shared<TEntity>( args... );
			entity->init();
			entity->setup();
			add_entity( entity );

			return entity;
		}
		void add_entity( SharedPtr<Entity> entity );
		void remove_entity( SharedPtr<Entity> entity );
		void clear_entities();

		void add_timer( const Timer& timer );

		IGame* get_game() const { return _game.get(); }
		Window* get_window() const { return _window.get(); }
		RenderBatch* get_render_batch() const { return _render_batch.get(); }
		InputManager* get_inputs() const { return _inputs.get(); }
		Physics* get_physics() const { return _physics.get(); }
		Updater* get_updater() { return &_updater; }

	public:
		/*
		 * Called when an entity is being added.
		 * 
		 * Parameters:
		 * - SafePtr<Entity> Entity being added
		 */
		Event<SafePtr<Entity>> on_entity_added;
		/*
		 * Called when an entity is being removed.
		 * 
		 * Parameters:
		 * - Entity* Entity being removed
		 */
		Event<Entity*> on_entity_removed;
		/*
		 * Called when ImGui windows are updating.
		 */
		Event<> on_imgui_update;

	public:
		Camera* camera { nullptr };
		bool is_debug = false;
		bool is_game_paused = false;

	private:
		bool _is_running = true;
		bool _is_updating = false;

	private:
		Engine() {};

		void process_input();
		void update( float dt );
		void render();

	private:
		std::vector<SharedPtr<Entity>> _pending_entities, _entities, _dead_entities;

		std::vector<Timer> _timers;

		std::unique_ptr<IGame> _game;
		std::unique_ptr<Window> _window;
		std::unique_ptr<RenderBatch> _render_batch;
		std::unique_ptr<InputManager> _inputs;
		std::unique_ptr<Physics> _physics;

		Updater _updater {};

		std::unique_ptr<Scene> _scene;
	};
}
