#pragma once

#include <suprengine/vec2.h>

#include <suprengine/assets.h>
#include <suprengine/updater.h>
#include <suprengine/scene.h>
#include <suprengine/timer.hpp>

#include <suprengine/input-manager.h>
#include <suprengine/physics.h>
#include <suprengine/game.h>

#include <suprengine/components/camera.h>

#include <vector>

namespace suprengine
{
	class Engine
	{
	public:
		//  ensure singleton
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

			//  create game
			auto game = new TGame();
			game->_engine = this;
			_game = std::unique_ptr<IGame>( game );

			//  get game infos
			auto infos = _game->get_infos();

			//  init window
			_window = std::make_unique<Window>( 
				infos.title, 
				infos.width, 
				infos.height
			);
			if ( !_window->init() ) return false;

			//  init render batch
			_render_batch = std::unique_ptr<RenderBatch>( 
				game->_create_render_batch( get_window() ) 
			);
			Assets::set_render_batch( get_render_batch() );
			if ( !_render_batch->init() ) return false;

			//  setup window size on render batch
			_render_batch->on_window_resized( _window->get_size() );

			//  init managers
			_inputs = std::make_unique<InputManager>();
			_physics = std::make_unique<Physics>();

			//  init game
			_game->load_assets();
			_game->init();

			return true;
		}
		void loop();

		template <typename TScene, typename ...Args>
		void create_scene( Args&& ...args )
		{
			static_assert( 
				std::is_base_of<Scene, TScene>::value, 
				"Engine::create_scene: used for a non-Scene class!"
			);

			//  reset
			clear_entities();

			//  set scene
			_scene = std::make_unique<TScene>( args... );
			_scene->init();
		}
		Scene* get_scene() const { return _scene.get(); }

		void add_entity( Entity* entity );
		void remove_entity( Entity* entity );
		void clear_entities();

		void add_timer( const Timer& timer );

		IGame* get_game() const { return _game.get(); }
		Window* get_window() const { return _window.get(); }
		RenderBatch* get_render_batch() const { return _render_batch.get(); }
		InputManager* get_inputs() const { return _inputs.get(); }
		Physics* get_physics() const { return _physics.get(); }
		Updater* get_timer() { return &_updater; }

	public:
		Camera* camera { nullptr };
		bool is_debug { false };

	private:
		Engine() {};

		void process_input();
		void update( float dt );
		void render();

		bool _is_running { true }, _is_updating { false };
		std::vector<Entity*> _pending_entities, _entities, _dead_entities;

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
