#pragma once

#include "assets.h"
#include "timer.h"
#include "vec2.h"
#include "ecs/entity.fwd.h"

#include <vector>
#include <unordered_set>

namespace suprengine
{
	enum class KeyState
	{
		UP,
		DOWN,
		PRESSED,
		RELEASED,
	};

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

		std::map<SDL_Scancode, KeyState> keystates;
		std::unordered_set<SDL_Scancode> survey_keys;
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

		bool is_key_pressed( SDL_Scancode code );
		bool is_key_released( SDL_Scancode code );
		bool is_key_down( SDL_Scancode code );
		bool is_key_up( SDL_Scancode code );
		KeyState get_key_state( SDL_Scancode code );

		Window* get_window() const { return _window; }
		RenderBatch* get_render_batch() const { return _render_batch; }
		Timer* get_timer() { return &_timer; }
	};
}
