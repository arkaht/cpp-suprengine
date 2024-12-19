#include "input-manager.h"

using namespace suprengine;

InputManager::InputManager()
{
	//	Reset current states so update() doesn't copy an array of unset numbers
	SDL_memset( current_states, 0, SDL_NUM_SCANCODES );

	update();
}

void InputManager::update()
{
	//  Reset mouse inputs
	mouse_delta = Vec2::zero;
	mouse_wheel = Vec2::zero;

	//	Copy keyboard current states into previous ones
	SDL_memcpy( previous_states, current_states, SDL_NUM_SCANCODES );

	//	Copy new states into current ones
	const uint8* state = SDL_GetKeyboardState( NULL );
	SDL_memcpy( current_states, state, SDL_NUM_SCANCODES );

	//	Update last mouse variables
	last_mouse_pos = current_mouse_pos;
	last_mouse_state = current_mouse_state;

	//	Update current mouse position
	int x, y;
	SDL_GetMouseState( &x, &y );

	current_mouse_pos.x = static_cast<float>( x );
	current_mouse_pos.y = static_cast<float>( y );
}

MouseButton InputManager::convert_sdl_mouse_button( uint8 button_index )
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

void InputManager::take_mouse_button_down( MouseButton button )
{
	if ( ( current_mouse_state & button ) == button ) return;

	current_mouse_state = current_mouse_state | button;
}

void InputManager::take_mouse_button_up( MouseButton button )
{
	if ( ( current_mouse_state & button ) == MouseButton::None ) return;

	current_mouse_state = current_mouse_state ^ button;
}

bool InputManager::is_key_just_pressed( SDL_Scancode key ) const
{ 
	return get_key_state( key ) == KeyState::Pressed; 
}

bool InputManager::is_key_just_released( SDL_Scancode key ) const
{ 
	return get_key_state( key ) == KeyState::Released; 
}

bool InputManager::is_key_pressed( SDL_Scancode key ) const
{
	KeyState state = get_key_state( key );
	return state == KeyState::Pressed || state == KeyState::Down;
}

bool InputManager::is_key_released( SDL_Scancode key ) const
{
	KeyState state = get_key_state( key );
	return state == KeyState::Released || state == KeyState::Up;
}

bool InputManager::is_key_up( SDL_Scancode key ) const
{ 
	return get_key_state( key ) == KeyState::Up; 
}

bool InputManager::is_key_down( SDL_Scancode key ) const
{ 
	return get_key_state( key ) == KeyState::Down; 
}

KeyState InputManager::get_key_state( SDL_Scancode key ) const
{
	if ( previous_states[key] )
	{
		if ( current_states[key] )
		{
			return KeyState::Down;
		}

		return KeyState::Released;
	}
	else if ( current_states[key] )
	{
		return KeyState::Pressed;
	}

	return KeyState::Up;
}

float InputManager::get_keys_as_axis( 
	SDL_Scancode negative_key,
	SDL_Scancode positive_key, 
	float value,
	float default_value
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

void InputManager::set_relative_mouse_mode( bool value )
{ 
	SDL_SetRelativeMouseMode( value ? SDL_TRUE : SDL_FALSE ); 
}

bool InputManager::is_mouse_button_just_pressed( MouseButton button ) const
{
	return get_mouse_button_state( button ) == KeyState::Pressed;
}

bool InputManager::is_mouse_button_just_released( MouseButton button ) const
{
	return get_mouse_button_state( button ) == KeyState::Released;
}

bool InputManager::is_mouse_button_pressed( MouseButton button ) const
{
	const KeyState state = get_mouse_button_state( button );
	return state == KeyState::Pressed || state == KeyState::Down;
}

bool InputManager::is_mouse_button_released( MouseButton button ) const
{
	const KeyState state = get_mouse_button_state( button );
	return state == KeyState::Released || state == KeyState::Up;
}

bool InputManager::is_mouse_button_up( MouseButton button ) const
{
	return get_mouse_button_state( button ) == KeyState::Up;
}

bool InputManager::is_mouse_button_down( MouseButton button ) const
{
	return get_mouse_button_state( button ) == KeyState::Down;
}

KeyState InputManager::get_mouse_button_state( MouseButton button ) const
{
	if ( ( last_mouse_state & button ) == button )
	{
		if ( ( current_mouse_state & button ) == button )
		{
			return KeyState::Down;
		}

		return KeyState::Released;
	}
	else if ( ( current_mouse_state & button ) == button )
	{
		return KeyState::Pressed;
	}

	return KeyState::Up;
}
