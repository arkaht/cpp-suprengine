#pragma once

#include <SDL.h>

namespace suprengine
{
	enum class KeyState
	{
		UP,
		DOWN,
		PRESSED,
		RELEASED,
	};

	class InputManager
	{
	private:
		uint8_t previous_states[SDL_NUM_SCANCODES];
		const uint8_t* current_states;

		uint32_t last_mouse_state, current_mouse_state;
		Vec2 last_mouse_pos{},
			 current_mouse_pos{};
	public:
		Vec2 mouse_delta {};

		InputManager()
		{
			SDL_memset( previous_states, 0, SDL_NUM_SCANCODES );  //  init everything to 0
			current_states = SDL_GetKeyboardState( NULL );

			int x, y;
			current_mouse_state = last_mouse_state = SDL_GetMouseState( &x, &y );
			current_mouse_pos = last_mouse_pos = { (float) x, (float) y };
		}

		void update()
		{
			SDL_memcpy( previous_states, current_states, SDL_NUM_SCANCODES );  //  copy current states into previous
			current_states = SDL_GetKeyboardState( NULL );

			int x, y;
			last_mouse_pos = current_mouse_pos;
			last_mouse_state = current_mouse_state;
			current_mouse_state = SDL_GetMouseState( &x, &y );
			current_mouse_pos = { (float) x, (float) y };
		}

		KeyState get_key_state( SDL_Scancode key )
		{
			if ( previous_states[key] )
			{
				if ( current_states[key] )
					return KeyState::DOWN;

				return KeyState::RELEASED;
			}
			else if ( current_states[key] )
				return KeyState::PRESSED;

			return KeyState::UP;
		}
		bool is_key_just_pressed( SDL_Scancode key ) { return get_key_state( key ) == KeyState::PRESSED; }  //  TODO: fix it, doesn't work
		bool is_key_just_released( SDL_Scancode key ) { return get_key_state( key ) == KeyState::RELEASED; }  //  TODO: fix it, doesn't work
		bool is_key_pressed( SDL_Scancode key ) 
		{
			KeyState state = get_key_state( key ); 
			return state == KeyState::PRESSED || state == KeyState::DOWN; 
		}
		bool is_key_released( SDL_Scancode key ) 
		{
			KeyState state = get_key_state( key );
			return state == KeyState::RELEASED || state == KeyState::UP;
		}
		bool is_key_up( SDL_Scancode key ) { return get_key_state( key ) == KeyState::UP; }
		bool is_key_down( SDL_Scancode key ) { return get_key_state( key ) == KeyState::DOWN; }

		void set_relative_mouse_mode( bool value ) { SDL_SetRelativeMouseMode( value ? SDL_TRUE : SDL_FALSE ); }
	};
}