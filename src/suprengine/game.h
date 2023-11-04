#pragma once
#include "game.fwd.h"

#include "assets.h"
#include "updater.h"
#include "vec2.h"
#include "scene.h"
#include "timer.hpp"
#include "input-manager.h"
#include "physics.h"
#include "entity.fwd.h"
#include "components/collider.fwd.h"
#include "components/camera.h"

#include <vector>

namespace suprengine
{
	class Game
	{
	public:
		Camera* camera { nullptr };
		bool is_debug { false };

		//  ensure singleton
		static Game& instance()
		{
			static Game inst;
			return inst;
		}

		Game( const Game& ) = delete;
		Game( Game&& ) = delete;
		Game& operator=( const Game& ) = delete;
		Game& operator=( Game&& ) = delete;
		~Game();

		template <typename TRenderBatch>
		bool init( const char* title = "my-cpp-game", int width = DEFAULT_WINDOW_WIDTH, int height = DEFAULT_WINDOW_HEIGHT )
		{
			static_assert( std::is_base_of<RenderBatch, TRenderBatch>::value, "Game::init: used for a non-RenderBatch class!" );

			//  init window
			_window = std::make_unique<Window>( title, width, height );
			if ( !_window->init() ) return false;

			//  init render batch
			_render_batch = std::make_unique<TRenderBatch>( get_window() );
			if ( !_render_batch->init() ) return false;

			//  init assets
			Assets::set_render_batch( get_render_batch() );

			//  init managers
			_inputs = std::make_unique<InputManager>();
			_physics = std::make_unique<Physics>();

			return true;
		}
		void loop();

		template <typename TScene, typename ...Args>
		void create_scene( Args&& ...args )
		{
			static_assert( std::is_base_of<Scene, TScene>::value, "Game::create_scene: used for a non-Scene class!" );

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

		Window* get_window() const { return _window.get(); }
		RenderBatch* get_render_batch() const { return _render_batch.get(); }
		InputManager* get_inputs() const { return _inputs.get(); }
		Physics* get_physics() const { return _physics.get(); }
		Updater* get_timer() { return &_updater; }
	
	private:
		Game() {};

		void process_input();
		void update( float dt );
		void render();

		bool _is_running { true }, _is_updating { false };
		std::vector<Entity*> _pending_entities, _entities, _dead_entities;

		std::vector<Timer> _timers;

		std::unique_ptr<Window> _window;
		std::unique_ptr<RenderBatch> _render_batch;
		std::unique_ptr<InputManager> _inputs;
		std::unique_ptr<Physics> _physics;

		Updater _updater {};

		std::unique_ptr<Scene> _scene;

	};
}
