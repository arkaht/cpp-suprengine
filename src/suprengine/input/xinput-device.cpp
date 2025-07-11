#include "xinput-device.h"

#include <limits>

#include "suprengine/core/engine.h"
#include "suprengine/math/vec2.h"

using namespace suprengine;

XInputDevice::XInputDevice( InputManager* manager ) : InputDevice( manager ) {}

void handle_joystick(
	const short joystick_x, const short joystick_y,
	const short deadzone,
	Vec2& out_joystick
)
{
	Vec2 joystick {
		static_cast<float>( joystick_x ),
		static_cast<float>( joystick_y )
	};

	const float length = joystick.length();
	if ( length > static_cast<float>( deadzone ) )
	{
		constexpr float MAX_SHORT_VALUE = std::numeric_limits<short>::max();
		if ( length > MAX_SHORT_VALUE )
		{
			joystick.normalize();
		}
		else
		{
			joystick /= MAX_SHORT_VALUE;
		}

		/*printf( "%f %f => %f (%f)\n",
			joystick.x, joystick.y,
			joystick.length(), length );*/
	}
	else
	{
		joystick = Vec2::zero;
	}

	out_joystick = joystick;
}

void handle_trigger( const uint8 trigger_value, float& out_trigger_value )
{
	out_trigger_value = static_cast<float>( trigger_value ) / 255.0f;
	/*printf( "Trigger: %f\n", out_trigger_value );*/
}

void XInputDevice::update()
{
	constexpr float RECONNECTION_DELAY = 1.0f;

	Engine& engine = Engine::instance();
	float game_time = engine.get_updater()->get_accumulated_seconds();

#ifdef PLATFORM_WINDOWS
	for ( int gamepad_id = 0; gamepad_id < XUSER_MAX_COUNT; gamepad_id++ )
	{
		XInputGamepadState& gamepad_state = _gamepads[gamepad_id];
		if ( !gamepad_state.is_connected
		   && gamepad_state.next_update_time > game_time )
		{
			continue;
		}

		// Getting gamepad state
		XINPUT_STATE& state = gamepad_state.internal_state;
		ZeroMemory( &state, sizeof( XINPUT_STATE ) );
		const DWORD result = XInputGetState( gamepad_id, &state );

		if ( result != ERROR_SUCCESS )
		{
			// Scheduling an update time for non-connected gamepads as specified
			// in XInput documentation.
			gamepad_state.is_connected = false;
			gamepad_state.next_update_time = game_time + RECONNECTION_DELAY;
			continue;
		}

		gamepad_state.is_connected = true;

		handle_joystick(
			state.Gamepad.sThumbLX, state.Gamepad.sThumbLY,
			XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE,
			_manager->left_gamepad_joystick
		);
		handle_joystick(
			state.Gamepad.sThumbRX, state.Gamepad.sThumbRY,
			XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE,
			_manager->right_gamepad_joystick
		);

		handle_trigger(
			state.Gamepad.bLeftTrigger,
			_manager->left_gamepad_trigger
		);
		handle_trigger(
			state.Gamepad.bRightTrigger,
			_manager->right_gamepad_trigger
		);

		// GamepadButton enum is purposely mapped to the same values as XInput buttons macros,
		// meaning casting this value is safe.
		const GamepadButton buttons = static_cast<GamepadButton>( state.Gamepad.wButtons );
		_manager->take_gamepad_button( buttons );
		/*printf("A pressed: %d\n", _manager->get_gamepad_button_state( GamepadButton::FaceButtonDown ) );

		printf("Gamepad %d is connected\n", gamepad_id);*/
	}
#endif
}
