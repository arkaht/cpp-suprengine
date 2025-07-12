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
	const auto& cursor = ImGui::GetCursorScreenPos();

	// Draw background
	draw_list->AddCircleFilled(
		ImVec2 {
			cursor.x + widget_size * 0.5f,
			cursor.y + button_radius,
		},
		button_radius,
		button_up ? IM_COL32( 200, 200, 0, 255 ) : background_color
	);
	draw_list->AddCircleFilled(
		ImVec2 {
			cursor.x + widget_size - button_radius,
			cursor.y + widget_size * 0.5f,
		},
		button_radius,
		button_right ? IM_COL32( 200, 0, 0, 255 ) : background_color
	);
	draw_list->AddCircleFilled(
		ImVec2 {
			cursor.x + widget_size * 0.5f,
			cursor.y + widget_size - button_radius,
		},
		button_radius,
		button_down ? IM_COL32( 0, 200, 0, 255 ) : background_color
	);
	draw_list->AddCircleFilled(
		ImVec2 {
			cursor.x + button_radius,
			cursor.y + widget_size * 0.5f,
		},
		button_radius,
		button_left ? IM_COL32( 0, 0, 200, 255 ) : background_color
	);

	ImGui::Dummy( ImVec2( widget_size, widget_size ) );
}

void DpadButtons(
	bool button_up, bool button_right,
	bool button_down, bool button_left,
	float widget_size = GAMEPAD_WIDGET_SIZE,
	float button_radius = FACE_BUTTONS_RADIUS,
	const uint32 background_color = GAMEPAD_WIDGETS_BACKGROUND_COLOR
)
{
	ImDrawList* draw_list = ImGui::GetWindowDrawList();
	const auto& cursor = ImGui::GetCursorScreenPos();

	// Draw background
	constexpr uint32 PRESSED_COLOR = IM_COL32( 255, 255, 255, 255 );
	draw_list->AddCircleFilled(
		ImVec2 {
			cursor.x + widget_size * 0.5f,
			cursor.y + button_radius,
		},
		button_radius,
		button_up ? PRESSED_COLOR : background_color
	);
	draw_list->AddCircleFilled(
		ImVec2 {
			cursor.x + widget_size - button_radius,
			cursor.y + widget_size * 0.5f,
		},
		button_radius,
		button_right ? PRESSED_COLOR : background_color
	);
	draw_list->AddCircleFilled(
		ImVec2 {
			cursor.x + widget_size * 0.5f,
			cursor.y + widget_size - button_radius,
		},
		button_radius,
		button_down ? PRESSED_COLOR : background_color
	);
	draw_list->AddCircleFilled(
		ImVec2 {
			cursor.x + button_radius,
			cursor.y + widget_size * 0.5f,
		},
		button_radius,
		button_left ? PRESSED_COLOR : background_color
	);

	ImGui::Dummy( ImVec2( widget_size, widget_size ) );
}

void XInputDevice::populate_imgui()
{
	InputManager* inputs = Engine::instance().get_inputs();

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
			ImGui::Text( "Left Joystick: %s", *Vec2::zero.to_string() );
			ImGui::Text( "Right Joystick: %s", *Vec2::zero.to_string() );

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

			ImGui::TreePop();
		}
		ImGui::PopID();

		connected_count++;
	}

	ImGui::Text( "Connected: %d", connected_count );
}
