#include "input-manager.h"

using namespace suprengine;

InputManager::InputManager()
{
	//  setup keyboard states
	SDL_memset( previous_states, 0, SDL_NUM_SCANCODES );  //  init everything to 0
	current_states = SDL_GetKeyboardState( NULL );

	//  setup mouse states
	int x, y;
	current_mouse_state = last_mouse_state = SDL_GetMouseState( &x, &y );
	current_mouse_pos = last_mouse_pos = { (float) x, (float) y };
}

void InputManager::update()
{
	SDL_memcpy( previous_states, current_states, SDL_NUM_SCANCODES );  //  copy current states into previous
	current_states = SDL_GetKeyboardState( NULL );

	int x, y;
	last_mouse_pos = current_mouse_pos;
	last_mouse_state = current_mouse_state;
	current_mouse_state = SDL_GetMouseState( &x, &y );
	current_mouse_pos = { (float) x, (float) y };
}

KeyState InputManager::get_key_state( SDL_Scancode key )
{
	if ( previous_states[key] )
	{
		if ( current_states[key] )
			return KeyState::DOWN;

		return KeyState::RELEASED;
	}
	else if ( current_states[key] )
		return KeyState::PRESSED;

	return KeyState::UP;
}

bool InputManager::is_key_just_pressed( SDL_Scancode key ) 
{ 
	return get_key_state( key ) == KeyState::PRESSED; 
}

bool InputManager::is_key_just_released( SDL_Scancode key ) 
{ 
	return get_key_state( key ) == KeyState::RELEASED; 
}

bool InputManager::is_key_pressed( SDL_Scancode key )
{
	KeyState state = get_key_state( key );
	return state == KeyState::PRESSED || state == KeyState::DOWN;
}

bool InputManager::is_key_released( SDL_Scancode key )
{
	KeyState state = get_key_state( key );
	return state == KeyState::RELEASED || state == KeyState::UP;
}

bool InputManager::is_key_up( SDL_Scancode key ) 
{ 
	return get_key_state( key ) == KeyState::UP; 
}

bool InputManager::is_key_down( SDL_Scancode key ) 
{ 
	return get_key_state( key ) == KeyState::DOWN; 
}

void InputManager::set_relative_mouse_mode( bool value ) 
{ 
	SDL_SetRelativeMouseMode( value ? SDL_TRUE : SDL_FALSE ); 
}
