#pragma once

#include <SDL.h>
#include <unordered_map>

#include <suprengine/math/vec2.h>

#include <suprengine/utils/usings.h>

#include <suprengine/input/buttons.h>
#include <suprengine/input/input-action.h>

namespace suprengine
{
	class InputDevice;

	class InputManager
	{
	public:
		InputManager();
		~InputManager();

		/*
		 * Update inputs and handle SDL events.
		 * Returns whenever the game can still run.
		 */
		bool update();

		void take_mouse_button_down( MouseButton button );
		void take_mouse_button_up( MouseButton button );

		void take_key_down( SDL_Scancode key );
		void take_gamepad_button( GamepadButton button );

		template <typename T>
		std::enable_if_t<is_input_action_valid<T>, InputAction<T>*> create_action( const std::string& name )
		{
			// TODO: Check that name doesn't conflict with one already existing
			InputAction<T>* input_action = new InputAction<T>( name );
			_input_actions.push_back( input_action );
			return input_action;
		}
		template <typename T>
		InputAction<T>* get_action( const std::string& name )
		{
			for ( InputActionBase* input_action : _input_actions )
			{
				if ( input_action->name == name )
				{
					return dynamic_cast<InputAction<T>*>( input_action );
				}
			}

			return nullptr;
		}

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

		bool is_gamepad_button_just_pressed( GamepadButton button ) const;
		bool is_gamepad_button_just_released( GamepadButton button ) const;
		bool is_gamepad_button_pressed( GamepadButton button ) const;
		bool is_gamepad_button_released( GamepadButton button ) const;
		bool is_gamepad_button_up( GamepadButton button ) const;
		bool is_gamepad_button_down( GamepadButton button ) const;
		KeyState get_gamepad_button_state( GamepadButton button ) const;

		void set_relative_mouse_mode( bool value );
		bool is_relative_mouse_mode_enabled() const;

		bool is_mouse_button_just_pressed( MouseButton button ) const;
		bool is_mouse_button_just_released( MouseButton button ) const;
		bool is_mouse_button_pressed( MouseButton button ) const;
		bool is_mouse_button_released( MouseButton button ) const;
		bool is_mouse_button_up( MouseButton button ) const;
		bool is_mouse_button_down( MouseButton button ) const;

		KeyState get_mouse_button_state( MouseButton button ) const;
		Vec2 get_mouse_pos() const
		{
			return _current_mouse_pos;
		}

		void populate_imgui();

	public:
		Vec2 mouse_delta {};
		Vec2 mouse_wheel {};

		Vec2 left_gamepad_joystick {};
		Vec2 right_gamepad_joystick {};

		float left_gamepad_trigger = 0.0f;
		float right_gamepad_trigger = 0.0f;

	private:
		/*
		 * Poll all SDL events and handle them.
		 * Returns whenever the game can still run.
		 */
		bool poll_events();

	private:
		uint8_t _previous_states[SDL_NUM_SCANCODES] {};
		uint8_t _current_states[SDL_NUM_SCANCODES] {};

		MouseButton _last_mouse_state = MouseButton::None;
		MouseButton _current_mouse_state = MouseButton::None;

		GamepadButton _current_gamepad_buttons = GamepadButton::None;
		GamepadButton _last_gamepad_buttons = GamepadButton::None;

		Vec2 _last_mouse_pos {};
		Vec2 _current_mouse_pos {};

		std::vector<InputDevice*> _input_devices {};
		std::vector<InputActionBase*> _input_actions {};
	};
}