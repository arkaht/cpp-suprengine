#pragma once

#include <SDL.h>

#include <suprengine/math/vec2.h>

#include <suprengine/utils/enum-flags.h>
#include <suprengine/utils/usings.h>

#include "input-device.h"

namespace suprengine
{
	enum class KeyState : uint8
	{
		Up		 = 0,
		Down	 = 1,
		Pressed	 = 2,
		Released = 3,
	};

	enum class MouseButton : uint8
	{
		None   = 0,
		Left   = 1 << 0,
		Middle = 1 << 1,
		Right  = 1 << 2,
	};
	DEFINE_ENUM_WITH_FLAGS( MouseButton, uint8 )

	/*
	 * Enum flags, holding all generic gamepad buttons.
	 * Values are purposely mapped to XInput buttons values for a better integration.
	 */
	enum class GamepadButton : uint16
	{
		None			= 0,
		DpadUp			= 1 << 0,
		DpadDown		= 1 << 1,
		DpadLeft		= 1 << 2,
		DpadRight		= 1 << 3,
		Start			= 1 << 4,
		Back			= 1 << 5,
		LeftThumb		= 1 << 6,
		RightThumb		= 1 << 7,
		LeftShoulder	= 1 << 8,
		RightShoulder	= 1 << 9,

		// XInput does not implement the trigger values as buttons.
		// Fortunately, the mapping of further values is still correct because it jumped over them.

		LeftTrigger		= 1 << 10,
		RightTrigger	= 1 << 11,

		FaceButtonDown	= 1 << 12,
		FaceButtonRight = 1 << 13,
		FaceButtonLeft	= 1 << 14,
		FaceButtonUp	= 1 << 15,
	};
	DEFINE_ENUM_WITH_FLAGS( GamepadButton, uint16 )

	class InputManager
	{
	public:
		InputManager();
		~InputManager();

		void update();

		MouseButton convert_sdl_mouse_button( uint8 button_index );

		void take_mouse_button_down( MouseButton button );
		void take_mouse_button_up( MouseButton button );

		void take_key_down( SDL_Scancode key );
		void take_gamepad_button( GamepadButton button );

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
		uint8_t _previous_states[SDL_NUM_SCANCODES] {};
		uint8_t _current_states[SDL_NUM_SCANCODES] {};

		MouseButton _last_mouse_state = MouseButton::None;
		MouseButton _current_mouse_state = MouseButton::None;

		GamepadButton _current_gamepad_buttons = GamepadButton::None;
		GamepadButton _last_gamepad_buttons = GamepadButton::None;

		Vec2 _last_mouse_pos {};
		Vec2 _current_mouse_pos {};

		std::vector<InputDevice*> _input_devices {};
	};
}