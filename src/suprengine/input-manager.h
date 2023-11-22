#pragma once

#include <SDL.h>

#include <suprengine/vec2.h>

namespace suprengine
{
	enum class KeyState
	{
		Up,
		Down,
		Pressed,
		Released,
	};

	enum class MouseButton
	{
		Left = SDL_BUTTON_LEFT,
		Middle = SDL_BUTTON_MIDDLE,
		Right = SDL_BUTTON_RIGHT,
	};

	class InputManager
	{
	public:
		Vec2 mouse_delta {};

		InputManager();

		void update();


		bool is_key_just_pressed( SDL_Scancode key );
		bool is_key_just_released( SDL_Scancode key );
		bool is_key_pressed( SDL_Scancode key );
		bool is_key_released( SDL_Scancode key );
		bool is_key_up( SDL_Scancode key );
		bool is_key_down( SDL_Scancode key );
		KeyState get_key_state( SDL_Scancode key );
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

		void set_relative_mouse_mode( bool value );
		bool is_mouse_button_just_pressed( MouseButton button );
		bool is_mouse_button_just_released( MouseButton button );
		bool is_mouse_button_pressed( MouseButton button );
		bool is_mouse_button_released( MouseButton button );
		bool is_mouse_button_up( MouseButton button );
		bool is_mouse_button_down( MouseButton button );
		KeyState get_mouse_button_state( MouseButton button );
		Vec2 get_mouse_pos() const { return current_mouse_pos; }

	private:
		uint8_t previous_states[SDL_NUM_SCANCODES];
		uint8_t current_states[SDL_NUM_SCANCODES];

		uint32_t last_mouse_state, current_mouse_state;
		Vec2 last_mouse_pos {};
		Vec2 current_mouse_pos {};
	};
}