#pragma once
#include "game.fwd.h"

#include "assets.h"
#include "updater.h"
#include "vec2.h"
#include "ecs/entity.fwd.h"
#include "ecs/components/collider.fwd.h"
#include "scene.fwd.h"
#include "timer.hpp"
#include "input-manager.h"
#include "physics.h"

#include <suprengine/ecs/components/camera.h>

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
			//  init window
			window = std::make_unique<Window>( title, width, height );
			if ( !window->init() ) return false;

			//  init render batch
			render_batch = std::make_unique<TRenderBatch>( get_window() );
			if ( !render_batch->init() ) return false;

			//  init assets
			Assets::set_render_batch( get_render_batch() );

			//  init managers
			inputs = std::make_unique<InputManager>();
			physics = std::make_unique<Physics>();

			return true;
		}
		void loop();

		void set_scene( Scene* scene );
		Scene* get_scene() const { return scene; }

		void add_entity( Entity* entity );
		void remove_entity( Entity* entity );
		void clear_entities();

		void add_timer( const Timer& timer );

		Window* get_window() const { return window.get(); }
		RenderBatch* get_render_batch() const { return render_batch.get(); }
		InputManager* get_inputs() { return inputs.get(); }
		Physics* get_physics() { return physics.get(); }
		Updater* get_timer() { return &updater; }
	
	private:
		Game() {};

		void process_input();
		void update( float dt );
		void render();

		bool _is_running { true }, _is_updating { false };
		std::vector<Entity*> pending_entities, entities, dead_entities;

		std::vector<Timer> timers;

		std::unique_ptr<Window> window;
		std::unique_ptr<RenderBatch> render_batch;
		std::unique_ptr<InputManager> inputs;
		std::unique_ptr<Physics> physics;

		Updater updater {};

		Scene* scene { nullptr };

	};
}
