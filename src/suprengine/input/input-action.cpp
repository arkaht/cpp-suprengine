#include "input-action.h"

#include "input-manager.h"
#include "suprengine/utils/imgui/imgui-input-visualizer.h"

using namespace suprengine;

/*
 * InputAction<bool>
 */

void InputAction<bool>::assign_mouse_button( MouseButton button )
{
	_assigned_mouse_button = button;
}

void InputAction<bool>::assign_key( const PhysicalKey key )
{
	_assigned_keys.push_back( key );
}

void InputAction<bool>::assign_gamepad_button( const GamepadButton gamepad_button )
{
	_assigned_gamepad_buttons.push_back( gamepad_button );
}

bool InputAction<bool>::read_value( const InputContext& context ) const
{
	if ( context.is_using_gamepad() )
	{
		// Evaluate gamepad buttons
		for ( const GamepadButton button : _assigned_gamepad_buttons )
		{
			if ( _inputs->is_gamepad_button_down( context.gamepad_id, button ) )
			{
				return true;
			}
		}
	}

	if ( context.is_using_keyboard_and_mouse() )
	{
		// Evaluate mouse button
		if ( _assigned_mouse_button != MouseButton::None )
		{
			if ( _inputs->is_mouse_button_down( _assigned_mouse_button ) )
			{
				return true;
			}
		}

		// Evaluate keyboard inputs
		for ( const PhysicalKey key : _assigned_keys )
		{
			if ( _inputs->is_key_down( key ) )
			{
				return true;
			}
		}
	}

	return false;
}

void InputAction<bool>::populate_imgui( const InputContext& context )
{
	const bool value = read_value( context );

	ImGui::InputVisualizer::Bool( value );

	ImGui::SameLine();
	ImGui::Text(
		"Name: %s\n"
		"Value: %s",
		*name,
		value ? "true" : "false"
	);
}

/*
 * InputAction<float>
 */

void InputAction<float>::assign_keys(
	const PhysicalKey negative_key,
	const PhysicalKey positive_key
)
{
	_assigned_negative_key = negative_key;
	_assigned_positive_key = positive_key;
}

void InputAction<float>::assign_gamepad_buttons(
	const GamepadButton negative_button,
	const GamepadButton positive_button
)
{
	_assigned_gamepad_negative_button = negative_button;
	_assigned_gamepad_positive_button = positive_button;
}

static float read_gamepad_button_value(
	const InputManager* inputs,
	const int gamepad_id,
	const GamepadButton button
)
{
	if ( inputs->is_gamepad_button_down( gamepad_id, button ) )
	{
		const bool is_left_trigger = enum_has_flag( button, GamepadButton::LeftTrigger );
		const bool is_right_trigger = enum_has_flag( button, GamepadButton::RightTrigger );
		if ( is_left_trigger )
		{
			return inputs->get_gamepad_trigger( gamepad_id, JoystickSide::Left );
		}
		if ( is_right_trigger )
		{
			return inputs->get_gamepad_trigger( gamepad_id, JoystickSide::Right );
		}

		return 1.0f;
	}

	return 0.0f;
}

float InputAction<float>::read_value( const InputContext& context ) const
{
	float value = 0.0f;

	if ( context.is_using_gamepad() )
	{
		if ( _assigned_gamepad_negative_button != GamepadButton::None
		  && _assigned_gamepad_positive_button != GamepadButton::None )
		{
			value =  read_gamepad_button_value( _inputs, context.gamepad_id, _assigned_gamepad_negative_button ) * -1.0f
				   + read_gamepad_button_value( _inputs, context.gamepad_id, _assigned_gamepad_positive_button );

			if ( value != 0.0f ) return value;
		}
	}

	if ( context.is_using_keyboard_and_mouse() )
	{
		if ( _assigned_negative_key != PhysicalKey::None
		  && _assigned_positive_key != PhysicalKey::None )
		{
			value = _inputs->get_keys_as_axis( _assigned_negative_key, _assigned_positive_key );
		}
	}

	return value;
}

void InputAction<float>::populate_imgui( const InputContext& context )
{
	const float value = read_value( context );

	ImGui::InputVisualizer::Axis( value );

	ImGui::SameLine();
	ImGui::Text(
		"Name: %s\n"
		"Value: %.2f",
		*name, value
	);
}

/*
 * InputAction<Vec2>
 */

void InputAction<Vec2>::assign_keys(
	Axis2D axis,
	const PhysicalKey negative_key,
	const PhysicalKey positive_key
)
{
	AxisKeys& keys = _keys_axes[static_cast<int>( axis )];
	keys.negative_key = negative_key;
	keys.positive_key = positive_key;
}

void InputAction<Vec2>::assign_gamepad_joystick(
	const JoystickSide side,
	const JoystickInputModifier modifier
)
{
	_joystick_side = side;
	_joystick_modifier = modifier;
}

void InputAction<Vec2>::assign_mouse_delta( const float multiplier )
{
	_mouse_movement			= true;
	_mouse_delta_multiplier = multiplier;
}

Vec2 InputAction<Vec2>::read_value( const InputContext& context ) const
{
	if ( context.is_using_gamepad() )
	{
		// Read gamepad inputs
		if ( _joystick_side != JoystickSide::None )
		{
			const Vec2& joystick = _inputs->get_gamepad_joystick( context.gamepad_id, _joystick_side );
			if ( joystick.length_sqr() != 0.0f )
			{
				return get_joystick_with_modifiers( joystick );
			}
		}
	}

	if ( context.is_using_keyboard_and_mouse() )
	{
		if ( _mouse_movement )
		{
			return _inputs->mouse_delta * _mouse_delta_multiplier;
		}

		// Read keyboard inputs
		Vec2 value = Vec2::zero;
		for ( int i = 0; i < 2; i++ )
		{
			const AxisKeys& keys = _keys_axes[i];
			if ( keys.negative_key == PhysicalKey::None
			  || keys.positive_key == PhysicalKey::None ) continue;

			value.set_axis(
				static_cast<Axis2D>( i ),
				_inputs->get_keys_as_axis( keys.negative_key, keys.positive_key )
			);
		}

		value.normalize();
		return value;
	}

	return Vec2::zero;
}

Vec2 InputAction<Vec2>::get_joystick_with_modifiers( Vec2 joystick ) const
{
	if ( enum_has_flag( _joystick_modifier, JoystickInputModifier::NegateX ) )
	{
		joystick.x = -joystick.x;
	}

	if ( enum_has_flag( _joystick_modifier, JoystickInputModifier::NegateY ) )
	{
		joystick.y = -joystick.y;
	}

	return joystick;
}

void InputAction<Vec2>::populate_imgui( const InputContext& context )
{
	const Vec2 value = read_value( context );

	ImGui::InputVisualizer::Joystick( value, 0.0f, false );

	ImGui::SameLine();
	ImGui::Text(
		"Name: %s\n"
		"X: %.2f\n"
		"Y: %.2f\n"
		"Length: %.2f",
		*name,
		value.x,
		value.y,
		value.length()
	);
}
