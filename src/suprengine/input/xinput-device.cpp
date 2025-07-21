#include "xinput-device.h"

#include <limits>

#include "suprengine/core/engine.h"
#include "suprengine/math/vec2.h"
#include "suprengine/utils/imgui/imgui-input-visualizer.h"

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
}

void XInputDevice::update()
{
	constexpr float RECONNECTION_DELAY = 1.0f;

	Engine& engine	= Engine::instance();
	float game_time = engine.get_updater()->get_accumulated_seconds();

#ifdef PLATFORM_WINDOWS
	for ( int i = 0; i < XUSER_MAX_COUNT; i++ )
	{
		XInputGamepadState& gamepad_state = _gamepads[i];
		if ( gamepad_state.gamepad_id == INDEX_NONE && gamepad_state.next_update_time > game_time )
		{
			continue;
		}

		// Getting gamepad state
		XINPUT_STATE& state = gamepad_state.internal_state;
		ZeroMemory( &state, sizeof( XINPUT_STATE ) );
		const DWORD result = XInputGetState( i, &state );

		if ( result != ERROR_SUCCESS )
		{
			if ( gamepad_state.gamepad_id != INDEX_NONE )
			{
				_manager->disconnect_gamepad( i );
			}

			// Scheduling an update time for non-connected gamepads as specified
			// in XInput documentation.
			gamepad_state.gamepad_id = INDEX_NONE;
			gamepad_state.next_update_time = game_time + RECONNECTION_DELAY;
			continue;
		}

		if ( gamepad_state.gamepad_id == INDEX_NONE )
		{
			const bool is_connected = _manager->connect_gamepad( gamepad_state.gamepad_id );
			if ( !is_connected )
			{
				gamepad_state.next_update_time = game_time + RECONNECTION_DELAY;
				continue;
			}
		}

		// TODO: Prevent direct access of GamepadInputs and use a proxy for each individual input

		GamepadInputs& gamepad_inputs = _manager->get_gamepad_inputs( gamepad_state.gamepad_id );

		handle_joystick(
			state.Gamepad.sThumbLX, state.Gamepad.sThumbLY,
			XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE,
			gamepad_inputs.left_joystick
		);
		handle_joystick(
			state.Gamepad.sThumbRX, state.Gamepad.sThumbRY,
			XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE,
			gamepad_inputs.right_joystick
		);

		handle_trigger(
			state.Gamepad.bLeftTrigger,
			XINPUT_GAMEPAD_TRIGGER_THRESHOLD,
			gamepad_inputs.left_trigger
		);
		handle_trigger(
			state.Gamepad.bRightTrigger,
			XINPUT_GAMEPAD_TRIGGER_THRESHOLD,
			gamepad_inputs.right_trigger
		);

		// GamepadButton enum is purposely mapped to the same values as XInput buttons macros,
		// meaning casting this value is safe.
		GamepadButton buttons = static_cast<GamepadButton>( state.Gamepad.wButtons );

		// Yet, we still have to manually add trigger buttons since they are not considered
		// as buttons by XInput.
		if ( gamepad_inputs.left_trigger > 0.0f )
		{
			buttons |= GamepadButton::LeftTrigger;
		}
		if ( gamepad_inputs.right_trigger > 0.0f )
		{
			buttons |= GamepadButton::RightTrigger;
		}

		gamepad_inputs.current_buttons |= buttons;
	}
#endif
}

void XInputDevice::populate_imgui()
{
	ImGui::SeparatorText( "XInput" );

	int connected_count = 0;
	for ( int i = 0; i < XUSER_MAX_COUNT; i++ )
	{
		const XInputGamepadState& gamepad_state = _gamepads[i];
		const bool is_connected = gamepad_state.gamepad_id != INDEX_NONE;

		// Open the very first node by default
		ImGui::SetNextItemOpen( true, ImGuiCond_Appearing );

		ImGui::PushID( i );
		if ( ImGui::TreeNode( "", "Gamepad %d: %s", i, is_connected ? "Connected" : "Disconnected" ) )
		{
			if ( !is_connected )
			{
				ImGui::TreePop();
				ImGui::PopID();
				continue;
			}

			const GamepadInputs& gamepad_inputs = _manager->get_gamepad_inputs( gamepad_state.gamepad_id );
			ImGui::Text( "Gamepad ID: %d", gamepad_state.gamepad_id );

			ImGui::Text(
				"Left Joystick: %s (%.0f%%)",
				*gamepad_inputs.left_joystick.to_string(),
				gamepad_inputs.left_joystick.length() * 100.0f
			);
			ImGui::Text(
				"Right Joystick: %s (%.0f%%)",
				*gamepad_inputs.right_joystick.to_string(),
				gamepad_inputs.right_joystick.length() * 100.0f
			);
			ImGui::Text( "Left Trigger: %.0f%%", gamepad_inputs.left_trigger * 100.0f );
			ImGui::Text( "Right Trigger: %.0f%%", gamepad_inputs.right_trigger * 100.0f );

			ImGui::InputVisualizer::Joystick(
				gamepad_inputs.left_joystick,
				static_cast<float>( XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE ) / std::numeric_limits<short>::max(),
				_manager->is_gamepad_button_pressed( gamepad_state.gamepad_id, GamepadButton::LeftThumb ),
				1
			);
			ImGui::SameLine();
			ImGui::InputVisualizer::Joystick(
				gamepad_inputs.right_joystick,
				static_cast<float>( XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE ) / std::numeric_limits<short>::max(),
				_manager->is_gamepad_button_pressed( gamepad_state.gamepad_id, GamepadButton::RightThumb ),
				1
			);

			ImGui::SameLine();
			ImGui::InputVisualizer::DpadButtons(
				_manager->is_gamepad_button_down( gamepad_state.gamepad_id, GamepadButton::DpadUp ),
				_manager->is_gamepad_button_down( gamepad_state.gamepad_id, GamepadButton::DpadRight ),
				_manager->is_gamepad_button_down( gamepad_state.gamepad_id, GamepadButton::DpadDown ),
				_manager->is_gamepad_button_down( gamepad_state.gamepad_id, GamepadButton::DpadLeft )
			);
			
			ImGui::SameLine();
			ImGui::InputVisualizer::FaceButtons(
				_manager->is_gamepad_button_down( gamepad_state.gamepad_id, GamepadButton::FaceButtonUp ),
				_manager->is_gamepad_button_down( gamepad_state.gamepad_id, GamepadButton::FaceButtonRight ),
				_manager->is_gamepad_button_down( gamepad_state.gamepad_id, GamepadButton::FaceButtonDown ),
				_manager->is_gamepad_button_down( gamepad_state.gamepad_id, GamepadButton::FaceButtonLeft )
			);

			ImGui::SameLine();
			ImGui::InputVisualizer::Shoulder(
				_manager->is_gamepad_button_down( gamepad_state.gamepad_id, GamepadButton::LeftTrigger ),
				_manager->is_gamepad_button_down( gamepad_state.gamepad_id, GamepadButton::LeftShoulder ),
				gamepad_inputs.right_trigger > 0.0f,
				_manager->is_gamepad_button_down( gamepad_state.gamepad_id, GamepadButton::RightShoulder )
			);
			ImGui::SameLine();
			ImGui::InputVisualizer::Shoulder(
				_manager->is_gamepad_button_down( gamepad_state.gamepad_id, GamepadButton::RightTrigger ),
				_manager->is_gamepad_button_down( gamepad_state.gamepad_id, GamepadButton::RightShoulder ),
				gamepad_inputs.left_trigger > 0.0f,
				_manager->is_gamepad_button_down( gamepad_state.gamepad_id, GamepadButton::LeftShoulder )
			);

			ImGui::TreePop();
		}
		ImGui::PopID();

		connected_count++;
	}

	ImGui::Text( "Connected: %d", connected_count );
}
