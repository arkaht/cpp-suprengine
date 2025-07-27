#pragma once

#include <vector>

#include <suprengine/input/buttons.h>
#include <suprengine/math/vec2.h>

namespace suprengine
{
	class InputManager;

	/*
	 * Structure defining which devices (e.g. K&M, gamepads) should be considered for reading inputs.
	 * Used by InputActions and stored in the InputComponent.
	 */
	struct InputContext
	{
	public:
		bool use_mouse_and_keyboard = false;
		int gamepad_id				= INDEX_NONE;

	public:
		bool is_using_gamepad() const
		{
			return gamepad_id != INDEX_NONE;
		}
		bool is_using_keyboard_and_mouse() const
		{
			return use_mouse_and_keyboard;
		}
	};

	class InputActionBase
	{
	public:
		explicit InputActionBase( const std::string& name, InputManager* inputs )
			: name( name ), _inputs( inputs ) {}
		virtual ~InputActionBase() = default;

		virtual void populate_imgui( const InputContext& context ) {};

	public:
		std::string name {};

	protected:
		InputManager* _inputs = nullptr;
	};

	template <typename T>
    class InputAction : public InputActionBase
	{
	public:
		InputAction() = delete;
	};

	/*
	 * An InputAction class is considered valid when it has a constructor accepting a
	 * const std::string& for setting his name and an InputManager* for reading inputs.
	 */
	template <typename T>
	constexpr bool is_input_action_valid = std::is_constructible_v<InputAction<T>, const std::string&, InputManager*>;

	template <>
	class InputAction<bool> : public InputActionBase
	{
	public:
		explicit InputAction( const std::string& name, InputManager* inputs )
			: InputActionBase( name, inputs ) {}

		/*
		 * Assign a mouse button that will trigger the action.
		 * Multiple calls to this function will override the previously assigned button.
		 */
		void assign_mouse_button( MouseButton button );
		/*
		 * Assign a new key that will trigger the action.
		 */
		void assign_key( SDL_Scancode key );
		/*
		 * Assign a new gamepad button that will trigger the action.
		 * Giving a value with multiple buttons will behave as an AND operator,
		 * meaning that all buttons have to be down to trigger the action.
		 */
		void assign_gamepad_button( GamepadButton gamepad_button );

		bool read_value( const InputContext& context ) const;

		void populate_imgui( const InputContext& context ) override;

	private:
		MouseButton _assigned_mouse_button = MouseButton::None;
		std::vector<SDL_Scancode> _assigned_keys {};
		std::vector<GamepadButton> _assigned_gamepad_buttons {};
	};

	enum class JoystickInputModifier : uint8
	{
		None	= 0,
		NegateX = 1 << 0,
		NegateY = 1 << 1,
	};
	DEFINE_ENUM_WITH_FLAGS( JoystickInputModifier, uint8 )

	template <>
	class InputAction<float> : public InputActionBase
	{
	public:
		explicit InputAction( const std::string& name, InputManager* inputs )
			: InputActionBase( name, inputs ) {}

		/*
		 * Assign two keyboard keys that will trigger the action.
		 * Multiple calls will override the previously assigned values.
		 */
		void assign_keys( SDL_Scancode negative_key, SDL_Scancode positive_key );
		/*
		 * Assign two gamepad buttons that will trigger the action.
		 * Giving a value with multiple buttons will behave as an AND operator,
		 * meaning that all buttons have to be down to trigger the action.
		 */
		void assign_gamepad_buttons( GamepadButton negative_button, GamepadButton positive_button );

		float read_value( const InputContext& context ) const;

		void populate_imgui( const InputContext& context ) override;

	private:
		SDL_Scancode _assigned_negative_key = SDL_SCANCODE_UNKNOWN;
		SDL_Scancode _assigned_positive_key = SDL_SCANCODE_UNKNOWN;
		GamepadButton _assigned_gamepad_negative_button = GamepadButton::None;
		GamepadButton _assigned_gamepad_positive_button = GamepadButton::None;
	};

	template <>
	class InputAction<Vec2> : public InputActionBase
	{
	public:
		explicit InputAction( const std::string& name, InputManager* inputs )
			: InputActionBase( name, inputs ) {}

		/*
		 * Assign an axis with two keys.
		 * Multiple calls with the same axis will override the previously assigned value.
		 */
		void assign_keys( Axis2D axis, SDL_Scancode negative_key, SDL_Scancode positive_key );
		void assign_gamepad_joystick( JoystickSide side, JoystickInputModifier modifier = JoystickInputModifier::None );
		void assign_mouse_delta( float multiplier = 0.05f );

		Vec2 read_value( const InputContext& context ) const;

		void populate_imgui( const InputContext& context ) override;

	private:
		Vec2 get_joystick_with_modifiers( Vec2 joystick ) const;

	private:
		struct AxisKeys
		{
			SDL_Scancode negative_key = SDL_SCANCODE_UNKNOWN;
			SDL_Scancode positive_key = SDL_SCANCODE_UNKNOWN;
		};

		AxisKeys _keys_axes[2] {};
		JoystickSide _joystick_side = JoystickSide::None;

		bool _mouse_movement = false;
		float _mouse_delta_multiplier = 0.0f;

		JoystickInputModifier _joystick_modifier = JoystickInputModifier::None;
	};
}