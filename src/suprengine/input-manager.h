#pragma once

#include <SDL.h>

#include <suprengine/vec2.h>

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
	public:
		Vec2 mouse_delta {};

		InputManager();

		void update();

		/*
		 * Returns a value in range [-value; value] representing the input axis
		 * of both keys. Keys need to be press to change the final value.
		 */
		float get_keys_as_axis( 
			SDL_Scancode negative_key, 
			SDL_Scancode positive_key, 
			float value = 1.0f,
			float default_value = 0.0f
		);

		KeyState get_key_state( SDL_Scancode key );
		bool is_key_just_pressed( SDL_Scancode key );  //  TODO: fix it, doesn't work
		bool is_key_just_released( SDL_Scancode key );  //  TODO: fix it, doesn't work
		bool is_key_pressed( SDL_Scancode key );
		bool is_key_released( SDL_Scancode key );
		bool is_key_up( SDL_Scancode key );
		bool is_key_down( SDL_Scancode key );

		void set_relative_mouse_mode( bool value );
		Vec2 get_mouse_pos() const { return current_mouse_pos; }

	private:
		uint8_t previous_states[SDL_NUM_SCANCODES];
		const uint8_t* current_states;

		uint32_t last_mouse_state, current_mouse_state;
		Vec2 last_mouse_pos {};
		Vec2 current_mouse_pos {};
	};
}