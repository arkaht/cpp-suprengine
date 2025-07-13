#include "input-action.h"

#include "input-manager.h"
#include "suprengine/utils/imgui.h"

using namespace suprengine;

/*
 * InputAction<bool>
 */

void InputAction<bool>::assign_mouse_button( MouseButton button )
{
	_assigned_mouse_button = button;
}

void InputAction<bool>::assign_key( const SDL_Scancode key )
{
	_assigned_keys.push_back( key );
}

void InputAction<bool>::assign_gamepad_button( const GamepadButton gamepad_button )
{
	_assigned_gamepad_buttons.push_back( gamepad_button );
}

bool InputAction<bool>::get_value() const
{
	return _value;
}

void InputAction<bool>::update( const InputManager* inputs )
{
	// Evaluate gamepad buttons
	for ( const GamepadButton button : _assigned_gamepad_buttons )
	{
		if ( inputs->is_gamepad_button_down( button ) )
		{
			_value = true;
			return;
		}
	}

	// Evaluate mouse button
	if ( _assigned_mouse_button != MouseButton::None )
	{
		if ( inputs->is_mouse_button_down( _assigned_mouse_button ) )
		{
			_value = true;
			return;
		}
	}

	// Evaluate keyboard inputs
	for ( const SDL_Scancode key : _assigned_keys )
	{
		if ( inputs->is_key_down( key ) )
		{
			_value = true;
			return;
		}
	}

	_value = false;
}

void InputAction<bool>::populate_imgui()
{
	bool readonly_value = _value;
	ImGui::Checkbox( "Value", &readonly_value );
	ImGui::Text( "Name: %s", *name );
}

/*
 * InputAction<Vec2>
 */

void InputAction<Vec2>::assign_keys(
	Axis2D axis,
	const SDL_Scancode negative_key,
	const SDL_Scancode positive_key
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
	_mouse_movement = true;
	_mouse_delta_multiplier = multiplier;
}

const Vec2& InputAction<Vec2>::get_value() const
{
	return _value;
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

void InputAction<Vec2>::update( const InputManager* inputs )
{
	_value = Vec2::zero;

	// Read gamepad inputs
	switch ( _joystick_side )
	{
		case JoystickSide::Left:
		{
			if ( inputs->left_gamepad_joystick.length_sqr() != 0.0f )
			{
				_value = get_joystick_with_modifiers( inputs->left_gamepad_joystick );
				return;
			}
			break;
		}
		case JoystickSide::Right:
		{
			if ( inputs->right_gamepad_joystick.length_sqr() != 0.0f )
			{
				_value = get_joystick_with_modifiers( inputs->right_gamepad_joystick );
				return;
			}
			break;
		}
		default:
			break;
	}

	if ( _mouse_movement )
	{
		_value.x = inputs->mouse_delta.x * _mouse_delta_multiplier;
		_value.y = inputs->mouse_delta.y * _mouse_delta_multiplier;
	}

	// Read keyboard inputs
	bool has_keyboard = false;
	for ( int i = 0; i < 2; i++ )
	{
		const AxisKeys& keys = _keys_axes[i];
		if ( keys.negative_key == SDL_SCANCODE_UNKNOWN
		  || keys.positive_key == SDL_SCANCODE_UNKNOWN ) continue;

		_value.set_axis(
			static_cast<Axis2D>( i ),
			inputs->get_keys_as_axis( keys.negative_key, keys.positive_key )
		);
		has_keyboard = true;
	}

	if ( has_keyboard )
	{
		_value.normalize();
	}
}

void InputAction<Vec2>::populate_imgui()
{
	ImGui::Extra::Joystick( _value, 0.0f, false );

	ImGui::SameLine();
	ImGui::Text(
		"Name: %s\n"
		"X: %.2f\n"
		"Y: %.2f\n"
		"Length: %.2f",
		*name, _value.x, _value.y, _value.length()
	);
}
