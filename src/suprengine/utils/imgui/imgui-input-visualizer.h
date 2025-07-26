#pragma once

#include <imgui.h>

#include "suprengine/math/vec2.h"
#include "suprengine/utils/usings.h"

namespace ImGui::InputVisualizer
{
	constexpr float GAMEPAD_WIDGET_SIZE	   = 48.0f;
	constexpr float JOYSTICK_WIDGET_RADIUS = GAMEPAD_WIDGET_SIZE / 2.0f;
	constexpr float JOYSTICK_RADIUS		   = 4.0f;
	constexpr float FACE_BUTTONS_RADIUS	   = 8.0f;

	constexpr uint32 KNOB_COLOR			  = IM_COL32( 215, 215, 215, 255 );
	constexpr uint32 DEADZONE_COLOR		  = IM_COL32( 255, 0,   0,   255 );
	constexpr uint32 AMPLITUDE_COLOR	  = IM_COL32( 255, 255, 0,   128 );
	constexpr uint32 VALID_COLOR		  = IM_COL32( 0,   255, 0,   128 );
	constexpr uint32 PRIMARY_GRID_COLOR	  = IM_COL32( 215, 215, 215, 128 );
	constexpr uint32 SECONDARY_GRID_COLOR = IM_COL32( 150, 150, 150, 128 );
	constexpr uint32 BACKGROUND_COLOR	  = IM_COL32( 0,   0,   0,   155 );

	enum class AxisKnobType
	{
		Slider,
		Radial,
	};

	enum class AxisDirection
	{
		Horizontal,
		Vertical,
	};

	void Axis(
		float value,
		float deadzone		   = 0.0f,
		int grid_divisions	   = 2,
		AxisKnobType knob_type = AxisKnobType::Radial,
		float widget_size	   = GAMEPAD_WIDGET_SIZE
	);

	void Bool(
		bool value,
		float widget_size = GAMEPAD_WIDGET_SIZE
	);

	void Joystick(
		suprengine::Vec2 joystick,
		float deadzone			= 0.0f,
		bool is_pressed			= false,
		int grid_divisions		= 2,
		float widget_radius		= JOYSTICK_WIDGET_RADIUS,
		float joystick_radius	= JOYSTICK_RADIUS,
		uint32 background_color = BACKGROUND_COLOR,
		uint32 joystick_color	= KNOB_COLOR
	);

	void FaceButtons(
		bool button_up,
		bool button_right,
		bool button_down,
		bool button_left,
		float widget_size		= GAMEPAD_WIDGET_SIZE,
		float button_radius		= FACE_BUTTONS_RADIUS,
		uint32 background_color = BACKGROUND_COLOR
	);

	void DpadButtons(
		bool button_up,
		bool button_right,
		bool button_down,
		bool button_left,
		float widget_size		= GAMEPAD_WIDGET_SIZE,
		float button_size		= FACE_BUTTONS_RADIUS * 2.0f,
		uint32 background_color = BACKGROUND_COLOR
	);

	// TODO: Think about displaying trigger amount and whether to display both triggers at once.
	void Shoulder(
		bool left_trigger,
		bool left_shoulder,
		bool right_trigger,
		bool right_shoulder,
		float widget_size		= GAMEPAD_WIDGET_SIZE,
		float button_size		= FACE_BUTTONS_RADIUS * 2.0f,
		uint32 background_color = BACKGROUND_COLOR
	);
}
