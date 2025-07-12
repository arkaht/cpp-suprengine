#include "input-manager.h"

#include "xinput-device.h"

using namespace suprengine;

InputManager::InputManager()
{
	//	Reset current states so update() doesn't copy an array of unset numbers
	SDL_memset( _current_states, 0, SDL_NUM_SCANCODES );

	_input_devices.push_back( new XInputDevice( this ) );

	update();
}

InputManager::~InputManager()
{
	for ( InputDevice* input_device : _input_devices )
	{
		delete input_device;
	}
}

void InputManager::update()
{
	//  Reset mouse input
	mouse_delta = Vec2::zero;
	mouse_wheel = Vec2::zero;

	//	Copy keyboard current states into previous ones
	SDL_memcpy( _previous_states, _current_states, SDL_NUM_SCANCODES );

	//	Copy new states into current ones
	const uint8* state = SDL_GetKeyboardState( nullptr );
	SDL_memcpy( _current_states, state, SDL_NUM_SCANCODES );

	//	Update last mouse variables
	_last_mouse_pos = _current_mouse_pos;
	_last_mouse_state = _current_mouse_state;

	//	Update current mouse position
	int x, y;
	SDL_GetMouseState( &x, &y );

	_current_mouse_pos.x = static_cast<float>( x );
	_current_mouse_pos.y = static_cast<float>( y );

	_last_gamepad_buttons = _current_gamepad_buttons;
	_current_gamepad_buttons = GamepadButton::None;

	for ( InputDevice* input_device : _input_devices )
	{
		input_device->update();
	}
}

MouseButton InputManager::convert_sdl_mouse_button( const uint8 button_index )
{
	switch ( button_index )
	{
		case 1:
			return MouseButton::Left;
		case 2:
			return MouseButton::Middle;
		case 3:
			return MouseButton::Right;
	}

	return MouseButton::None;
}

void InputManager::take_mouse_button_down( const MouseButton button )
{
	if ( ( _current_mouse_state & button ) == button ) return;

	_current_mouse_state = _current_mouse_state | button;
}

void InputManager::take_mouse_button_up( const MouseButton button )
{
	if ( ( _current_mouse_state & button ) == MouseButton::None ) return;

	_current_mouse_state = _current_mouse_state ^ button;
}

void InputManager::take_key_down( const SDL_Scancode key )
{
	_current_states[key] = true;
}

void InputManager::take_gamepad_button( const GamepadButton button )
{
	_current_gamepad_buttons |= button;
}

bool InputManager::is_key_just_pressed( const SDL_Scancode key ) const
{ 
	return get_key_state( key ) == KeyState::Pressed; 
}

bool InputManager::is_key_just_released( const SDL_Scancode key ) const
{ 
	return get_key_state( key ) == KeyState::Released; 
}

bool InputManager::is_key_pressed( const SDL_Scancode key ) const
{
	const KeyState state = get_key_state( key );
	return state == KeyState::Pressed || state == KeyState::Down;
}

bool InputManager::is_key_released( const SDL_Scancode key ) const
{
	const KeyState state = get_key_state( key );
	return state == KeyState::Released || state == KeyState::Up;
}

bool InputManager::is_key_up( const SDL_Scancode key ) const
{ 
	return get_key_state( key ) == KeyState::Up; 
}

bool InputManager::is_key_down( const SDL_Scancode key ) const
{ 
	return get_key_state( key ) == KeyState::Down; 
}

KeyState InputManager::get_key_state( const SDL_Scancode key ) const
{
	if ( _previous_states[key] )
	{
		if ( _current_states[key] )
		{
			return KeyState::Down;
		}

		return KeyState::Released;
	}

	if ( _current_states[key] )
	{
		return KeyState::Pressed;
	}

	return KeyState::Up;
}

float InputManager::get_keys_as_axis(
	const SDL_Scancode negative_key, const SDL_Scancode positive_key,
	const float value, const float default_value
) const
{
	float axis = default_value;

	if ( is_key_down( positive_key ) )
	{
		axis += value;
	}
	if ( is_key_down( negative_key ) )
	{
		axis -= value;
	}

	return axis;
}

bool InputManager::is_gamepad_button_just_pressed( const GamepadButton button ) const
{
	return get_gamepad_button_state( button ) == KeyState::Pressed;
}

bool InputManager::is_gamepad_button_just_released( const GamepadButton button ) const
{
	return get_gamepad_button_state( button ) == KeyState::Released;
}

bool InputManager::is_gamepad_button_pressed( const GamepadButton button ) const
{
	const KeyState state = get_gamepad_button_state( button );
	return state == KeyState::Pressed || state == KeyState::Down;
}

bool InputManager::is_gamepad_button_released( const GamepadButton button ) const
{
	const KeyState state = get_gamepad_button_state( button );
	return state == KeyState::Released || state == KeyState::Up;
}

bool InputManager::is_gamepad_button_up( const GamepadButton button ) const
{
	return get_gamepad_button_state( button ) == KeyState::Up;
}

bool InputManager::is_gamepad_button_down( const GamepadButton button ) const
{
	return get_gamepad_button_state( button ) == KeyState::Down;
}

KeyState InputManager::get_gamepad_button_state( const GamepadButton button ) const
{
	if ( enum_has_flag( _last_gamepad_buttons, button ) )
	{
		if ( enum_has_flag( _current_gamepad_buttons, button ) )
		{
			return KeyState::Down;
		}

		return KeyState::Released;
	}

	if ( enum_has_flag( _current_gamepad_buttons, button ) )
	{
		return KeyState::Pressed;
	}

	return KeyState::Up;
}

void InputManager::set_relative_mouse_mode( bool value )
{
	SDL_SetRelativeMouseMode( value ? SDL_TRUE : SDL_FALSE ); 
}

bool InputManager::is_relative_mouse_mode_enabled() const
{
	return SDL_GetRelativeMouseMode();;
}

bool InputManager::is_mouse_button_just_pressed( const MouseButton button ) const
{
	return get_mouse_button_state( button ) == KeyState::Pressed;
}

bool InputManager::is_mouse_button_just_released( const MouseButton button ) const
{
	return get_mouse_button_state( button ) == KeyState::Released;
}

bool InputManager::is_mouse_button_pressed( const MouseButton button ) const
{
	const KeyState state = get_mouse_button_state( button );
	return state == KeyState::Pressed || state == KeyState::Down;
}

bool InputManager::is_mouse_button_released( const MouseButton button ) const
{
	const KeyState state = get_mouse_button_state( button );
	return state == KeyState::Released || state == KeyState::Up;
}

bool InputManager::is_mouse_button_up( const MouseButton button ) const
{
	return get_mouse_button_state( button ) == KeyState::Up;
}

bool InputManager::is_mouse_button_down( const MouseButton button ) const
{
	return get_mouse_button_state( button ) == KeyState::Down;
}

KeyState InputManager::get_mouse_button_state( const MouseButton button ) const
{
	if ( enum_has_flag( _last_mouse_state, button ) )
	{
		if ( enum_has_flag( _current_mouse_state, button ) )
		{
			return KeyState::Down;
		}

		return KeyState::Released;
	}

	if ( enum_has_flag( _current_mouse_state, button ) )
	{
		return KeyState::Pressed;
	}

	return KeyState::Up;
}

void InputManager::populate_imgui()
{
	for ( InputDevice* input_device : _input_devices )
	{
		input_device->populate_imgui();
	}
}
