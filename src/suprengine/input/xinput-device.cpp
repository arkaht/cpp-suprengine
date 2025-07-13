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

constexpr float GAMEPAD_WIDGET_SIZE = 50.0f;
constexpr float JOYSTICK_WIDGET_RADIUS = GAMEPAD_WIDGET_SIZE / 2.0f;
constexpr float JOYSTICK_RADIUS = 5.0f;
constexpr float FACE_BUTTONS_RADIUS = 8.0f;
constexpr uint32 GAMEPAD_WIDGETS_BACKGROUND_COLOR = IM_COL32( 0, 0, 0, 155 );

void Joystick(
	const Vec2& joystick,
	float deadzone = 0.0f,
	bool is_pressed = false,
	float widget_radius = JOYSTICK_WIDGET_RADIUS, float joystick_radius = JOYSTICK_RADIUS,
	uint32 background_color = GAMEPAD_WIDGETS_BACKGROUND_COLOR,
	uint32 joystick_color = IM_COL32( 215, 215, 215, 255 ),
	uint32 amplitude_color = IM_COL32( 255, 255, 255, 128 )
)
{
	ImDrawList* draw_list = ImGui::GetWindowDrawList();
	const auto& cursor = ImGui::GetCursorScreenPos();

	// Draw background
	const ImVec2 background_circle_center = ImVec2( cursor.x + widget_radius, cursor.y + widget_radius );
	draw_list->AddCircleFilled( background_circle_center, widget_radius, background_color );

	// Draw deadzone
	if ( deadzone > 0.0f )
	{
		draw_list->AddCircle(
			background_circle_center,
			widget_radius * deadzone,
			IM_COL32( 255, 0, 0, 255 )
		);
	}

	// Draw amplitude
	const float amplitude = joystick.length();
	if ( amplitude > 0.0f )
	{
		draw_list->AddCircle(
			background_circle_center,
			widget_radius * amplitude,
			amplitude_color
		);
	}

	// Draw joystick
	const ImVec2 joystick_center {
		joystick.x * ( widget_radius - joystick_radius ) + cursor.x + widget_radius,
		-joystick.y * ( widget_radius - joystick_radius ) + cursor.y + widget_radius,
	};
	draw_list->AddCircleFilled( joystick_center, joystick_radius, joystick_color );

	if ( is_pressed )
	{
		draw_list->AddCircle( joystick_center, joystick_radius * 1.5f, joystick_color );
	}

	ImGui::Dummy( ImVec2( widget_radius * 2.0f, widget_radius * 2.0f ) );
};

void FaceButtons(
	bool button_up, bool button_right,
	bool button_down, bool button_left,
	float widget_size = GAMEPAD_WIDGET_SIZE,
	float button_radius = FACE_BUTTONS_RADIUS,
	const uint32 background_color = GAMEPAD_WIDGETS_BACKGROUND_COLOR
)
{
	ImDrawList* draw_list = ImGui::GetWindowDrawList();
	const ImVec2 cursor	  = ImGui::GetCursorScreenPos();
	const ImVec2 center   = ImVec2 {
		cursor.x + widget_size * 0.5f,
		cursor.y + widget_size * 0.5f
	};

	const bool buttons[4] { button_up, button_right, button_down, button_left };
	uint32 colors[4]
	{
		IM_COL32( 200, 200, 0, 255 ),
		IM_COL32( 200, 0,   0, 255 ),
		IM_COL32( 0,   200, 0, 255 ),
		IM_COL32( 0,   0,   200, 255 ),
	};
	constexpr ImVec2 directions[4]
	{
		ImVec2 {  0.0f, -1.0f },
		ImVec2 {  1.0f, 0.0f },
		ImVec2 {  0.0f, 1.0f },
		ImVec2 { -1.0f, 0.0f },
	};

	const float offset = widget_size * 0.5f - button_radius;
	for ( int i = 0; i < 4; i++ )
	{
		draw_list->AddCircleFilled(
			ImVec2 {
				center.x + directions[i].x * offset,
				center.y + directions[i].y * offset,
			},
			button_radius,
			buttons[i] ? colors[i] : background_color
		);
	}

	ImGui::Dummy( ImVec2( widget_size, widget_size ) );
}

void DpadButtons(
	bool button_up, bool button_right,
	bool button_down, bool button_left,
	float widget_size = GAMEPAD_WIDGET_SIZE,
	float button_size = FACE_BUTTONS_RADIUS * 2.0f,
	const uint32 background_color = GAMEPAD_WIDGETS_BACKGROUND_COLOR
)
{
	ImDrawList* draw_list = ImGui::GetWindowDrawList();
	const ImVec2 cursor	  = ImGui::GetCursorScreenPos();
	const ImVec2 center   = ImVec2 {
		cursor.x + widget_size * 0.5f,
		cursor.y + widget_size * 0.5f
	};

	const bool buttons[4] { button_up, button_right, button_down, button_left };
	constexpr ImVec2 directions[4]
	{
		ImVec2 {  0.0f, -1.0f },
		ImVec2 {  1.0f, 0.0f },
		ImVec2 {  0.0f, 1.0f },
		ImVec2 { -1.0f, 0.0f },
	};

	constexpr uint32 PRESSED_COLOR = IM_COL32( 255, 255, 255, 255 );
	const float offset = widget_size * 0.5f;
	for ( int i = 0; i < 4; i++ )
	{
		const float side_offset = offset - button_size;
		const ImVec2 side_extrude {
			directions[i].y * button_size * 0.5f,
			directions[i].x * button_size * 0.5f
		};

		draw_list->AddTriangleFilled(
			ImVec2 {
				center.x + directions[i].x * offset,
				center.y + directions[i].y * offset,
			},
			ImVec2 {
				center.x + directions[i].x * side_offset + side_extrude.x,
				center.y + directions[i].y * side_offset + side_extrude.y,
			},
			ImVec2 {
				center.x + directions[i].x * side_offset - side_extrude.x,
				center.y + directions[i].y * side_offset - side_extrude.y,
			},
			buttons[i] ? PRESSED_COLOR : background_color
		);
	}

	ImGui::Dummy( ImVec2( widget_size, widget_size ) );
}

void Shoulder(
	bool left_trigger, bool left_shoulder,
	bool right_trigger, bool right_shoulder,
	float widget_size = GAMEPAD_WIDGET_SIZE,
	float button_size = FACE_BUTTONS_RADIUS * 2.0f,
	const uint32 background_color = GAMEPAD_WIDGETS_BACKGROUND_COLOR
)
{
	ImDrawList* draw_list = ImGui::GetWindowDrawList();
	const ImVec2 cursor	  = ImGui::GetCursorScreenPos();

	constexpr uint32 PRESSED_COLOR = IM_COL32( 255, 255, 255, 255 );

	draw_list->AddRectFilled(
		ImVec2 { cursor.x, cursor.y + widget_size * 0.7f },
		ImVec2 { cursor.x + widget_size, cursor.y + widget_size },
		left_shoulder ? PRESSED_COLOR : background_color
	);
	draw_list->AddRectFilled(
		ImVec2 { cursor.x + widget_size * 0.5f - widget_size * 0.15f, cursor.y },
		ImVec2 { cursor.x + widget_size * 0.5f + widget_size * 0.15f, cursor.y + widget_size * 0.6f },
		left_trigger ? PRESSED_COLOR : background_color
	);

	ImGui::Dummy( ImVec2( widget_size, widget_size ) );
}

void XInputDevice::populate_imgui()
{
	const InputManager* inputs = Engine::instance().get_inputs();

	ImGui::SeparatorText( "XInput" );

	int connected_count = 0;
	for ( int gamepad_id = 0; gamepad_id < XUSER_MAX_COUNT; gamepad_id++ )
	{
		const XInputGamepadState& gamepad_state = _gamepads[gamepad_id];
		if ( !gamepad_state.is_connected ) return;

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

			Joystick(
				inputs->left_gamepad_joystick,
				static_cast<float>( XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE ) / std::numeric_limits<short>::max(),
				inputs->is_gamepad_button_pressed( GamepadButton::LeftThumb )
			);
			ImGui::SameLine();
			Joystick(
				inputs->right_gamepad_joystick,
				static_cast<float>( XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE ) / std::numeric_limits<short>::max(),
				inputs->is_gamepad_button_pressed( GamepadButton::RightThumb )
			);

			ImGui::SameLine();
			DpadButtons(
				inputs->is_gamepad_button_down( GamepadButton::DpadUp ),
				inputs->is_gamepad_button_down( GamepadButton::DpadRight ),
				inputs->is_gamepad_button_down( GamepadButton::DpadDown ),
				inputs->is_gamepad_button_down( GamepadButton::DpadLeft )
			);
			
			ImGui::SameLine();
			FaceButtons(
				inputs->is_gamepad_button_down( GamepadButton::FaceButtonUp ),
				inputs->is_gamepad_button_down( GamepadButton::FaceButtonRight ),
				inputs->is_gamepad_button_down( GamepadButton::FaceButtonDown ),
				inputs->is_gamepad_button_down( GamepadButton::FaceButtonLeft )
			);

			ImGui::SameLine();
			Shoulder(
				inputs->is_gamepad_button_down( GamepadButton::LeftTrigger ),
				inputs->is_gamepad_button_down( GamepadButton::LeftShoulder ),
				inputs->right_gamepad_trigger,
				inputs->is_gamepad_button_down( GamepadButton::RightShoulder )
			);
			ImGui::SameLine();
			Shoulder(
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
