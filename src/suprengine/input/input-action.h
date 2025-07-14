#pragma once

#include <vector>

#include <suprengine/input/buttons.h>
#include <suprengine/math/vec2.h>

namespace suprengine
{
	class InputManager;

	class InputActionBase
	{
	public:
		explicit InputActionBase( const std::string& name )
			: name( name ) {}
		virtual ~InputActionBase() = default;

		virtual void update( const InputManager* inputs ) = 0;

		virtual void populate_imgui() {};

	public:
		std::string name {};
	};

	template <typename T>
    class InputAction : public InputActionBase
	{
	public:
		InputAction() = delete;
	};

	/*
	 * An InputAction class is considered valid when it has a constructor accepting a
	 * const std::string& for setting his name.
	 */
	template <typename T>
	constexpr bool is_input_action_valid = std::is_constructible_v<InputAction<T>, const std::string&>;

	enum class JoystickSide
	{
		None,
		Left,
		Right,
	};

	template <>
	class InputAction<bool> : public InputActionBase
	{
	public:
		explicit InputAction( const std::string& name ) : InputActionBase( name ) {}

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

		bool get_value() const;

		void update( const InputManager* inputs ) override;

		void populate_imgui() override;

	private:
		MouseButton _assigned_mouse_button = MouseButton::None;
		std::vector<SDL_Scancode> _assigned_keys {};
		std::vector<GamepadButton> _assigned_gamepad_buttons {};

		bool _value = false;
	};

	// TODO: InputAction<float>

	enum class JoystickInputModifier : uint8
	{
		None	= 0,
		NegateX = 1 << 0,
		NegateY = 1 << 1,
	};
	DEFINE_ENUM_WITH_FLAGS( JoystickInputModifier, uint8 )

	template <>
	class InputAction<Vec2> : public InputActionBase
	{
	public:
		explicit InputAction( const std::string& name ) : InputActionBase( name ) {}

		/*
		 * Assign an axis with two keys.
		 * Multiple calls with the same axis will override the previously assigned value.
		 */
		void assign_keys( Axis2D axis, SDL_Scancode negative_key, SDL_Scancode positive_key );
		void assign_gamepad_joystick( JoystickSide side, JoystickInputModifier modifier = JoystickInputModifier::None );
		void assign_mouse_delta( float multiplier = 0.05f );

		const Vec2& get_value() const;

		void update( const InputManager* inputs ) override;

		void populate_imgui() override;

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

		Vec2 _value = Vec2::zero;

		JoystickInputModifier _joystick_modifier = JoystickInputModifier::None;
	};
}