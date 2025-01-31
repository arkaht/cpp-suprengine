#pragma once

#include <SDL.h>

#include <suprengine/math/vec2.h>

#include <suprengine/utils/enum-flags.h>
#include <suprengine/utils/usings.h>

namespace suprengine
{
	enum class KeyState
	{
		Up,
		Down,
		Pressed,
		Released,
	};

	enum class MouseButton : uint8
	{
		None = 0,
		Left = 1,
		Middle = 2,
		Right = 4,
	};
	DEFINE_ENUM_WITH_FLAGS( MouseButton, uint8 )

	class InputManager
	{
	public:
		InputManager();

		void update();

		MouseButton convert_sdl_mouse_button( uint8 button_index );

		void take_mouse_button_down( MouseButton button );
		void take_mouse_button_up( MouseButton button );

		bool is_key_just_pressed( SDL_Scancode key ) const;
		bool is_key_just_released( SDL_Scancode key ) const;
		bool is_key_pressed( SDL_Scancode key ) const;
		bool is_key_released( SDL_Scancode key ) const;
		bool is_key_up( SDL_Scancode key ) const;
		bool is_key_down( SDL_Scancode key ) const;
		KeyState get_key_state( SDL_Scancode key ) const;
		/*
		 * Returns a value in range [-value; value] representing the input axis
		 * of both keys. Keys need to be press to change the final value.
		 */
		float get_keys_as_axis( 
			SDL_Scancode negative_key, 
			SDL_Scancode positive_key, 
			float value = 1.0f,
			float default_value = 0.0f
		) const;

		void set_relative_mouse_mode( bool value );
		bool is_mouse_button_just_pressed( MouseButton button ) const;
		bool is_mouse_button_just_released( MouseButton button ) const;
		bool is_mouse_button_pressed( MouseButton button ) const;
		bool is_mouse_button_released( MouseButton button ) const;
		bool is_mouse_button_up( MouseButton button ) const;
		bool is_mouse_button_down( MouseButton button ) const;

		KeyState get_mouse_button_state( MouseButton button ) const;
		Vec2 get_mouse_pos() const { return current_mouse_pos; }

	public:
		Vec2 mouse_delta {};
		Vec2 mouse_wheel {};

	private:
		uint8_t previous_states[SDL_NUM_SCANCODES];
		uint8_t current_states[SDL_NUM_SCANCODES];

		MouseButton last_mouse_state = MouseButton::None;
		MouseButton current_mouse_state = MouseButton::None;

		Vec2 last_mouse_pos {};
		Vec2 current_mouse_pos {};
	};
}