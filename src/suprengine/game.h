#pragma once

#include "assets.h"
#include "timer.h"
#include "vec2.h"
#include "ecs/entity.fwd.h"

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

		Window* _window { nullptr };
		RenderBatch* _render_batch { nullptr };
		Timer _timer {};
	public:
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

		bool initialize( const char* title = "my-cpp-game", int width = DEFAULT_WINDOW_WIDTH, int height = DEFAULT_WINDOW_HEIGHT );
		void loop();

		void add_entity( Entity* entity );
		void remove_entity( Entity* entity );

		Window* get_window() const { return _window; }
		RenderBatch* get_render_batch() const { return _render_batch; }
		Timer* get_timer() { return &_timer; }
	};
}
