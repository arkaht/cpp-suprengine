#include "xinput-device.h"

#include <limits>

#include "suprengine/core/engine.h"
#include "suprengine/math/vec2.h"

using namespace suprengine;

XInputDevice::XInputDevice( InputManager* manager )
	: InputDevice( manager ) {}

void handle_joystick(
	const short joystick_x, const short joystick_y,
	const short deadzone,
	Vec2& out_joystick
)
{
	constexpr float MAX_SHORT_VALUE = std::numeric_limits<short>::max();
	Vec2 joystick {
		static_cast<float>( joystick_x ) / MAX_SHORT_VALUE,
		static_cast<float>( joystick_y ) / MAX_SHORT_VALUE,
	};

	const float length = joystick.length();
	if ( length < static_cast<float>( deadzone ) / MAX_SHORT_VALUE )
	{
		joystick = Vec2::zero;
	}
	else if ( length > 1.0f )
	{
		joystick /= length;
	}

	/*printf( "%f %f => %f (%f)\n",
		joystick.x, joystick.y,
		joystick.length(), length );*/

	out_joystick = joystick;
}

void handle_trigger( const uint8 trigger_value, const uint8 deadzone, float& out_trigger_value )
{
	if ( trigger_value < deadzone )
	{
		out_trigger_value = 0.0f;
		return;
	}

	out_trigger_value = static_cast<float>( trigger_value ) / 255.0f;
	/*printf( "Trigger: %f\n", out_trigger_value );*/
}

void XInputDevice::update()
{
	constexpr float RECONNECTION_DELAY = 1.0f;

	Engine& engine	= Engine::instance();
	float game_time = engine.get_updater()->get_accumulated_seconds();

#ifdef PLATFORM_WINDOWS
	for ( int gamepad_id = 0; gamepad_id < XUSER_MAX_COUNT; gamepad_id++ )
	{
		XInputGamepadState& gamepad_state = _gamepads[gamepad_id];
		if ( !gamepad_state.is_connected && gamepad_state.next_update_time > game_time )
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
			XINPUT_GAMEPAD_TRIGGER_THRESHOLD,
			_manager->left_gamepad_trigger
		);
		handle_trigger(
			state.Gamepad.bRightTrigger,
			XINPUT_GAMEPAD_TRIGGER_THRESHOLD,
			_manager->right_gamepad_trigger
		);

		// GamepadButton enum is purposely mapped to the same values as XInput buttons macros,
		// meaning casting this value is safe.
		GamepadButton buttons = static_cast<GamepadButton>( state.Gamepad.wButtons );

		// Yet, we still have to manually add trigger buttons since they are not considered
		// as buttons by XInput.
		if ( _manager->left_gamepad_trigger > 0.0f )
		{
			buttons |= GamepadButton::LeftTrigger;
		}
		if ( _manager->right_gamepad_trigger > 0.0f )
		{
			buttons |= GamepadButton::RightTrigger;
		}

		_manager->take_gamepad_button( buttons );
		/*printf("A pressed: %d\n", _manager->get_gamepad_button_state(
		 * GamepadButton::FaceButtonDown ) );*/

		/*printf("Gamepad %d is connected\n", gamepad_id);*/
	}
#endif
}

void XInputDevice::populate_imgui()
{
	const InputManager* inputs = Engine::instance().get_inputs();

	ImGui::SeparatorText( "XInput" );

	int connected_count = 0;
	for ( int gamepad_id = 0; gamepad_id < XUSER_MAX_COUNT; gamepad_id++ )
	{
		const XInputGamepadState& gamepad_state = _gamepads[gamepad_id];
		if ( !gamepad_state.is_connected )
		{
			ImGui::PushID( gamepad_id );
			if ( ImGui::TreeNode( "", "Gamepad %d: Disconnected", gamepad_id ) )
			{
				ImGui::TreePop();
			}
			ImGui::PopID();
			continue;
		}

		// Open the very first node by default
		if ( gamepad_id == 0 )
		{
			ImGui::SetNextItemOpen( true, ImGuiCond_Appearing );
		}

		ImGui::PushID( gamepad_id );
		if ( ImGui::TreeNode( "", "Gamepad %d", gamepad_id ) )
		{
			ImGui::Text(
				"Left Joystick: %s (%.0f%%)",
				*inputs->left_gamepad_joystick.to_string(),
				inputs->left_gamepad_joystick.length() * 100.0f
			);
			ImGui::Text(
				"Right Joystick: %s (%.0f%%)",
				*inputs->right_gamepad_joystick.to_string(),
				inputs->right_gamepad_joystick.length() * 100.0f
			);
			ImGui::Text( "Left Trigger: %.0f%%", inputs->left_gamepad_trigger * 100.0f );
			ImGui::Text( "Right Trigger: %.0f%%", inputs->right_gamepad_trigger * 100.0f );

			ImGui::Extra::Joystick(
				inputs->left_gamepad_joystick,
				static_cast<float>( XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE ) / std::numeric_limits<short>::max(),
				inputs->is_gamepad_button_pressed( GamepadButton::LeftThumb )
			);
			ImGui::SameLine();
			ImGui::Extra::Joystick(
				inputs->right_gamepad_joystick,
				static_cast<float>( XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE ) / std::numeric_limits<short>::max(),
				inputs->is_gamepad_button_pressed( GamepadButton::RightThumb )
			);

			ImGui::SameLine();
			ImGui::Extra::DpadButtons(
				inputs->is_gamepad_button_down( GamepadButton::DpadUp ),
				inputs->is_gamepad_button_down( GamepadButton::DpadRight ),
				inputs->is_gamepad_button_down( GamepadButton::DpadDown ),
				inputs->is_gamepad_button_down( GamepadButton::DpadLeft )
			);
			
			ImGui::SameLine();
			ImGui::Extra::FaceButtons(
				inputs->is_gamepad_button_down( GamepadButton::FaceButtonUp ),
				inputs->is_gamepad_button_down( GamepadButton::FaceButtonRight ),
				inputs->is_gamepad_button_down( GamepadButton::FaceButtonDown ),
				inputs->is_gamepad_button_down( GamepadButton::FaceButtonLeft )
			);

			ImGui::SameLine();
			ImGui::Extra::Shoulder(
				inputs->is_gamepad_button_down( GamepadButton::LeftTrigger ),
				inputs->is_gamepad_button_down( GamepadButton::LeftShoulder ),
				inputs->right_gamepad_trigger,
				inputs->is_gamepad_button_down( GamepadButton::RightShoulder )
			);
			ImGui::SameLine();
			ImGui::Extra::Shoulder(
				inputs->is_gamepad_button_down( GamepadButton::RightTrigger ),
				inputs->is_gamepad_button_down( GamepadButton::RightShoulder ),
				inputs->left_gamepad_trigger,
				inputs->is_gamepad_button_down( GamepadButton::LeftShoulder )
			);

			ImGui::TreePop();
		}
		ImGui::PopID();

		connected_count++;
	}

	ImGui::Text( "Connected: %d", connected_count );
}
