#include "input-manager.h"

using namespace suprengine;

InputManager::InputManager()
{
	//  setup keyboard states
	SDL_memset( previous_states, 0, SDL_NUM_SCANCODES );  //  init everything to 0
	SDL_memset( current_states, 0, SDL_NUM_SCANCODES );

	//  setup mouse states
	int x, y;
	current_mouse_state = last_mouse_state = SDL_GetMouseState( &x, &y );
	current_mouse_pos = last_mouse_pos = { (float) x, (float) y };
}

void InputManager::update()
{
	SDL_memcpy( previous_states, current_states, SDL_NUM_SCANCODES );  //  copy current states into previous
	auto state = SDL_GetKeyboardState( NULL );
	SDL_memcpy( current_states, state, SDL_NUM_SCANCODES );  //  copy new state into current

	int x, y;
	last_mouse_pos = current_mouse_pos;
	last_mouse_state = current_mouse_state;
	current_mouse_state = SDL_GetMouseState( &x, &y );
	current_mouse_pos = { (float) x, (float) y };
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
			return KeyState::Down;

		return KeyState::Released;
	}
	else if ( current_states[key] )
		return KeyState::Pressed;

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
	KeyState state = get_mouse_button_state( button );
	return state == KeyState::Pressed || state == KeyState::Down;
}

bool InputManager::is_mouse_button_released( MouseButton button ) const
{
	KeyState state = get_mouse_button_state( button );
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
	auto mask = SDL_BUTTON( (int)button );

	if ( last_mouse_state & mask )
	{
		if ( current_mouse_state & mask )
		{
			return KeyState::Down;
		}

		return KeyState::Released;
	}
	else if ( current_mouse_state & mask )
	{
		return KeyState::Pressed;
	}

	return KeyState::Up;
}
