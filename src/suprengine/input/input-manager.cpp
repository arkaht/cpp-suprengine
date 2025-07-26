#include "input-manager.h"

#include "backends/imgui_impl_sdl2.h"

#include <suprengine/input/xinput-device.h>
#include <suprengine/utils/assert.h>

using namespace suprengine;

InputManager::InputManager()
{
	//	Reset current states so update() doesn't copy an array of unset numbers
	SDL_memset( _current_keyboard_states, 0, SDL_NUM_SCANCODES );

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
	SDL_memcpy( _last_keyboard_states, _current_keyboard_states, SDL_NUM_SCANCODES );

	//	Copy new states into current ones
	const uint8* state = SDL_GetKeyboardState( nullptr );
	SDL_memcpy( _current_keyboard_states, state, SDL_NUM_SCANCODES );

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

	// Update gamepads inputs
	for ( GamepadInputs& gamepad_inputs : _gamepads_inputs )
	{
		gamepad_inputs.last_buttons = gamepad_inputs.current_buttons;
		gamepad_inputs.current_buttons = GamepadButton::None;
	}

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
	_current_keyboard_states[key] = true;
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
	if ( _last_keyboard_states[key] )
	{
		if ( _current_keyboard_states[key] )
		{
			return KeyState::Down;
		}

		return KeyState::Released;
	}

	if ( _current_keyboard_states[key] )
	{
		return KeyState::Pressed;
	}

	return KeyState::Up;
}

float InputManager::get_keys_as_axis(
	const SDL_Scancode negative_key,
	const SDL_Scancode positive_key,
	const float value,
	const float default_value
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

bool InputManager::find_next_gamepad_id( int& gamepad_id ) const
{
	gamepad_id = INDEX_NONE;

	for ( int i = 0; i < MAX_GAMEPADS_COUNT; i++ )
	{
		if ( !_gamepads_inputs[i].is_connected )
		{
			gamepad_id = i;
			break;
		}
	}

	return gamepad_id != INDEX_NONE;
}

bool InputManager::connect_gamepad( int& gamepad_id )
{
	if ( !find_next_gamepad_id( gamepad_id ) )
	{
		Logger::error( "A gamepad couldn't connect: reached the maximum gamepad count!", gamepad_id );
		return false;
	}

	GamepadInputs& gamepad_inputs = get_gamepad_inputs( gamepad_id );
	ASSERT( !gamepad_inputs.is_connected );

	gamepad_inputs.is_connected = true;

	Logger::info( "Gamepad %d is connected!", gamepad_id );
	return true;
}

void InputManager::disconnect_gamepad( const int gamepad_id )
{
	GamepadInputs& gamepad_inputs = get_gamepad_inputs( gamepad_id );
	ASSERT( gamepad_inputs.is_connected );

	gamepad_inputs.is_connected = false;

	Logger::info( "Gamepad %d has been disconnected!", gamepad_id );
}

void InputManager::take_gamepad_button( const int gamepad_id, const GamepadButton button )
{
	GamepadInputs& gamepad_inputs = get_gamepad_inputs( gamepad_id );
	ASSERT( gamepad_inputs.is_connected );

	gamepad_inputs.current_buttons |= button;
}

bool InputManager::is_gamepad_connected( const int gamepad_id ) const
{
	return get_gamepad_inputs( gamepad_id ).is_connected;
}

GamepadInputs& InputManager::get_gamepad_inputs( const int gamepad_id )
{
	ASSERT( gamepad_id >= 0 && gamepad_id < MAX_GAMEPADS_COUNT );
	return _gamepads_inputs[gamepad_id];
}

const GamepadInputs& InputManager::get_gamepad_inputs( const int gamepad_id ) const
{
	ASSERT( gamepad_id >= 0 && gamepad_id < MAX_GAMEPADS_COUNT );
	return _gamepads_inputs[gamepad_id];
}

bool InputManager::is_gamepad_button_just_pressed( const int gamepad_id, const GamepadButton button ) const
{
	return get_gamepad_button_state( gamepad_id, button ) == KeyState::Pressed;
}

bool InputManager::is_gamepad_button_just_released( const int gamepad_id, const GamepadButton button ) const
{
	return get_gamepad_button_state( gamepad_id, button ) == KeyState::Released;
}

bool InputManager::is_gamepad_button_pressed( const int gamepad_id, const GamepadButton button ) const
{
	const KeyState state = get_gamepad_button_state( gamepad_id, button );
	return state == KeyState::Pressed || state == KeyState::Down;
}

bool InputManager::is_gamepad_button_released( const int gamepad_id, const GamepadButton button ) const
{
	const KeyState state = get_gamepad_button_state( gamepad_id, button );
	return state == KeyState::Released || state == KeyState::Up;
}

bool InputManager::is_gamepad_button_up( const int gamepad_id, const GamepadButton button ) const
{
	return get_gamepad_button_state( gamepad_id, button ) == KeyState::Up;
}

bool InputManager::is_gamepad_button_down( const int gamepad_id, const GamepadButton button ) const
{
	return get_gamepad_button_state( gamepad_id, button ) == KeyState::Down;
}

KeyState InputManager::get_gamepad_button_state(
	const int gamepad_id,
	const GamepadButton button
) const
{
	const GamepadInputs& gamepad_inputs = get_gamepad_inputs( gamepad_id );
	if ( !gamepad_inputs.is_connected )
	{
		return KeyState::Up;
	}

	if ( enum_has_flag( gamepad_inputs.last_buttons, button ) )
	{
		if ( enum_has_flag( gamepad_inputs.current_buttons, button ) )
		{
			return KeyState::Down;
		}

		return KeyState::Released;
	}

	if ( enum_has_flag( gamepad_inputs.current_buttons, button ) )
	{
		return KeyState::Pressed;
	}

	return KeyState::Up;
}

const Vec2& InputManager::get_gamepad_joystick(
	const int gamepad_id,
	const JoystickSide side
) const
{
	const GamepadInputs& gamepad_inputs = get_gamepad_inputs( gamepad_id );
	if ( !gamepad_inputs.is_connected )
	{
		return Vec2::zero;
	}

	switch ( side )
	{
		case JoystickSide::Left:
			return gamepad_inputs.left_joystick;
		case JoystickSide::Right:
			return gamepad_inputs.right_joystick;
	}

	ASSERT( side != JoystickSide::None );
	return Vec2::zero;
}

float InputManager::get_gamepad_trigger( const int gamepad_id, const JoystickSide side ) const
{
	const GamepadInputs& gamepad_inputs = get_gamepad_inputs( gamepad_id );
	if ( !gamepad_inputs.is_connected )
	{
		return 0.0f;
	}

	switch ( side )
	{
		case JoystickSide::Left:
			return gamepad_inputs.left_trigger;
		case JoystickSide::Right:
			return gamepad_inputs.right_trigger;
	}

	ASSERT( side != JoystickSide::None );
	return 0.0f;
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

		ImGui::Columns( 2, "InputActions" );
		for ( InputActionBase* input_action : _input_actions )
		{
			input_action->populate_imgui();
			ImGui::NextColumn();
		}
		ImGui::Columns( 1 );

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
