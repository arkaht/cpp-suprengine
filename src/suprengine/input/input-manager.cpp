#include "input-manager.h"

#include "backends/imgui_impl_sdl2.h"

#include <suprengine/input/xinput-device.h>

using namespace suprengine;

InputManager::InputManager()
{
	//	Reset current states so update() doesn't copy an array of unset numbers
	SDL_memset( _current_states, 0, SDL_NUM_SCANCODES );

	_input_devices.push_back( new XInputDevice( this ) );
}

InputManager::~InputManager()
{
	for ( InputActionBase* input_action : _input_actions )
	{
		delete input_action;
	}

	for ( InputDevice* input_device : _input_devices )
	{
		delete input_device;
	}
}

bool InputManager::update()
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

	// Update mouse delta
	SDL_GetRelativeMouseState( &x, &y );
	mouse_delta.x = static_cast<float>( x );
	mouse_delta.y = static_cast<float>( y );

	_last_gamepad_buttons = _current_gamepad_buttons;
	_current_gamepad_buttons = GamepadButton::None;

	if ( !poll_events() ) return false;

	for ( InputDevice* input_device : _input_devices )
	{
		input_device->update();
	}

	for ( InputActionBase* input_action : _input_actions )
	{
		input_action->update( this );
	}

	return true;
}

bool InputManager::poll_events()
{
	const ImGuiIO& imgui_io = ImGui::GetIO();

	//  Read window events
	SDL_Event event;
	while ( SDL_PollEvent( &event ) )
	{
		//  Send event to ImGui
		ImGui_ImplSDL2_ProcessEvent( &event );

		switch ( event.type )
		{
			case SDL_MOUSEBUTTONDOWN:
			{
				if ( imgui_io.WantCaptureMouse ) continue;

				const MouseButton button = sdl_to_mouse_button( event.button.button );
				take_mouse_button_down( button );

				break;
			}
			case SDL_MOUSEBUTTONUP:
			{
				//	NOTE: We are not checking for ImGui's IO here in order to update the mouse
				//	button even when the user is releasing it on an ImGui interface, which would
				//	cause bugs otherwise.

				const MouseButton button = sdl_to_mouse_button( event.button.button );
				take_mouse_button_up( button );

				break;
			}
			case SDL_MOUSEWHEEL:
			{
				if ( imgui_io.WantCaptureMouse ) continue;

				//  Store mouse wheel for this frame
				mouse_wheel.x = static_cast<float>( event.wheel.x );
				mouse_wheel.y = static_cast<float>( event.wheel.y );
				break;
			}
			case SDL_QUIT:
			{
				//  Quit game when closing window
				return false;
			}
		}
	}

	return true;
}

void InputManager::take_mouse_button_down( const MouseButton button )
{
	if ( enum_has_flag( _current_mouse_state, button ) ) return;

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
	ImGui::SetNextItemOpen( true, ImGuiCond_Appearing );
	if ( ImGui::TreeNode( "Actions" ) )
	{
		ImGui::Text( "Input Actions: %d", _input_actions.size() );

		for ( InputActionBase* input_action : _input_actions )
		{
			input_action->populate_imgui();
		}

		ImGui::TreePop();
	}

	ImGui::SetNextItemOpen( true, ImGuiCond_Appearing );
	if ( ImGui::TreeNode( "Devices" ) )
	{
		for ( InputDevice* input_device : _input_devices )
		{
			input_device->populate_imgui();
		}

		ImGui::TreePop();
	}
}
