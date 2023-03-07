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

#include <suprengine/ecs/components/camera.h>

#include <vector>

namespace suprengine
{
	class Game
	{
	private:
		Game() {};

		void process_input();
		void update( float dt );
		void render();

		bool _is_running { true }, _is_updating { false };
		std::vector<Entity*> pending_entities, entities, dead_entities;
		std::vector<Collider*> colliders;

		std::vector<Timer> timers;

		Window* window { nullptr };
		RenderBatch* render_batch { nullptr };
		InputManager* inputs { nullptr };

		Updater updater {};

		Scene* scene { nullptr };

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
		Game& operator=( const Game& ) = delete;
		Game( Game&& ) = delete;
		Game& operator=( Game&& ) = delete;
		~Game();

		template <typename TRenderBatch>
		bool initialize( const char* title = "my-cpp-game", int width = DEFAULT_WINDOW_WIDTH, int height = DEFAULT_WINDOW_HEIGHT )
		{
			//  init window
			window = new Window( title, width, height );
			if ( !window->initialize() ) return false;

			//  init render batch
			render_batch = new TRenderBatch( window );
			if ( !render_batch->initialize() ) return false;

			//  init assets
			Assets::set_render_batch( render_batch );

			//  init input manager
			inputs = new InputManager();

			return true;
		}
		void loop();

		void set_scene( Scene* scene );
		Scene* get_scene() const { return scene; }

		void add_entity( Entity* entity );
		void remove_entity( Entity* entity );
		void clear_entities();

		void add_timer( const Timer& timer );

		void add_collider( Collider* collider );
		void remove_collider( Collider* collider );

		Window* get_window() const { return window; }
		RenderBatch* get_render_batch() const { return render_batch; }
		Updater* get_timer() { return &updater; }
		InputManager* get_inputs() { return inputs; }
	};
}
