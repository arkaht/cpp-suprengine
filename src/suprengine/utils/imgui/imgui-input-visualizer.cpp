#include "imgui-input-visualizer.h"

#include <suprengine/math/math.h>

static uint32 get_amplitude_color_of_value( const float value )
{
	if ( value < -1.0f || value > 1.0f ) return ImGui::InputVisualizer::DEADZONE_COLOR;
	if ( value == -1.0f || value == 1.0f ) return ImGui::InputVisualizer::VALID_COLOR;

	return ImGui::InputVisualizer::AMPLITUDE_COLOR;
}

void ImGui::InputVisualizer::Axis(
	const float value,
	const float deadzone,
	const int grid_divisions,
	const AxisKnobType knob_type,
	const float widget_size
)
{
	ImDrawList* draw_list = ImGui::GetWindowDrawList();
	const ImVec2& cursor  = ImGui::GetCursorScreenPos();

	const ImVec2 center = ImVec2 {
		cursor.x + widget_size * 0.5f,
		cursor.y + widget_size * 0.5f
	};
	const float background_height = widget_size;

	draw_list->AddRectFilled(
		ImVec2 {
			cursor.x,
			center.y - background_height * 0.5f,
		},
		ImVec2 {
			cursor.x + widget_size,
			center.y + background_height * 0.5f,
		},
		BACKGROUND_COLOR
	);

	const ImVec2 knob_size {
		knob_type == AxisKnobType::Slider ? widget_size * 0.08f : JOYSTICK_RADIUS * 2.0f,
		knob_type == AxisKnobType::Slider ? widget_size * 0.2f : JOYSTICK_RADIUS * 2.0f
	};

	draw_list->AddLine(
		ImVec2 {
			cursor.x,
			center.y,
		},
		ImVec2 {
			cursor.x + widget_size,
			center.y,
		},
		SECONDARY_GRID_COLOR
	);

	// Draw grid lines
	const float axis_viewport_width = widget_size - knob_size.x;
	if ( grid_divisions > 0 )
	{
		const float grid_divisions_as_float = static_cast<float>( grid_divisions ) * 2.0f;
		const float division_width			= axis_viewport_width / grid_divisions_as_float;
		for ( int i = -grid_divisions; i <= grid_divisions; i++ )
		{
			const float x = cursor.x + widget_size * 0.5f
						  + division_width * static_cast<float>( i );

			draw_list->AddLine(
				ImVec2 {
					x,
					center.y - background_height * 0.5f,
				},
				ImVec2 {
					x,
					center.y + background_height * 0.5f,
				},
				i % 2 == 0 ? PRIMARY_GRID_COLOR : SECONDARY_GRID_COLOR
			);
		}
	}

	if ( !suprengine::math::near_zero( value ) )
	{
		const float amplitude_x = center.x + value * axis_viewport_width * 0.5f;
		draw_list->AddLine(
			ImVec2 {
				amplitude_x,
				center.y - background_height * 0.5f,
			},
			ImVec2 {
				amplitude_x,
				center.y + background_height * 0.5f,
			},
			get_amplitude_color_of_value( value )
		);
	}

	const ImVec2 slider_position {
		center.x + axis_viewport_width * 0.5f * value,
		center.y
	};

	switch ( knob_type )
	{
		case AxisKnobType::Slider:
			draw_list->AddRectFilled(
				ImVec2 {
					slider_position.x - knob_size.x * 0.5f,
					slider_position.y - knob_size.y * 0.5f
				},
				ImVec2 {
					slider_position.x + knob_size.x * 0.5f,
					slider_position.y + knob_size.y * 0.5f
				},
				KNOB_COLOR
			);
			break;
		case AxisKnobType::Radial:
			draw_list->AddCircleFilled( slider_position, JOYSTICK_RADIUS, KNOB_COLOR );
			break;
	}

	ImGui::Dummy( ImVec2( widget_size, widget_size ) );
}

void ImGui::InputVisualizer::Bool( const bool value, const float widget_size )
{
	ImDrawList* draw_list = ImGui::GetWindowDrawList();
	const ImVec2 cursor   = ImGui::GetCursorScreenPos();
	const ImVec2 center   = ImVec2 {
		cursor.x + widget_size * 0.5f,
		cursor.y + widget_size * 0.5f
	};

	draw_list->AddCircleFilled( center, widget_size * 0.5f, BACKGROUND_COLOR );

	draw_list->AddCircleFilled( center, widget_size * 0.4f, value ? VALID_COLOR : IM_COL32( 30, 30, 30, 128 ) );

	ImGui::Dummy( ImVec2( widget_size, widget_size ) );
}

void ImGui::InputVisualizer::Joystick(
	suprengine::Vec2 joystick,
	const float deadzone,
	const bool is_pressed,
	const int grid_divisions,
	const float widget_radius,
	const float joystick_radius,
	const uint32 background_color,
	const uint32 joystick_color
)
{
	ImDrawList* draw_list = ImGui::GetWindowDrawList();
	const ImVec2& cursor  = ImGui::GetCursorScreenPos();

	// Draw background
	const ImVec2 center = ImVec2 {
		cursor.x + widget_radius,
		cursor.y + widget_radius
	};
	draw_list->AddCircleFilled( center, widget_radius, background_color );

	// Draw cross
	draw_list->AddLine(
		ImVec2 {
			center.x - joystick_radius,
			center.y,
		},
		ImVec2 {
			center.x + joystick_radius,
			center.y,
		},
		joystick_color
	);
	draw_list->AddLine(
		ImVec2 {
			center.x,
			center.y - joystick_radius,
		},
		ImVec2 {
			center.x,
			center.y + joystick_radius,
		},
		joystick_color
	);

	// Draw grid lines
	if ( grid_divisions > 0 )
	{
		const float division_radius = widget_radius / static_cast<float>( grid_divisions );
		for ( int i = 0; i < grid_divisions + 1; i++ )
		{
			draw_list->AddCircle(
				center,
				static_cast<float>( i ) * division_radius,
				i % 2 == 0 ? PRIMARY_GRID_COLOR : SECONDARY_GRID_COLOR
			);
		}

		draw_list->AddLine(
			ImVec2 { cursor.x, center.y },
			ImVec2 { cursor.x + widget_radius * 2.0f, center.y },
			PRIMARY_GRID_COLOR
		);
		draw_list->AddLine(
			ImVec2 { center.x, cursor.y },
			ImVec2 { center.x, cursor.y + widget_radius * 2.0f },
			PRIMARY_GRID_COLOR
		);
	}

	// Draw deadzone
	if ( deadzone > 0.0f )
	{
		draw_list->AddCircle(
			center,
			suprengine::math::ceil( widget_radius * deadzone ),
			DEADZONE_COLOR
		);
	}

	// Draw amplitude
	float amplitude = joystick.length();
	if ( amplitude > 0.0f )
	{
		const float pre_normalized_amplitude = amplitude;

		if ( amplitude > 1.0f )
		{
			joystick /= amplitude;
			amplitude	 = 1.0f;
		}

		draw_list->AddCircle(
			center,
			widget_radius * amplitude,
			get_amplitude_color_of_value( pre_normalized_amplitude )
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
}

void ImGui::InputVisualizer::FaceButtons(
	bool button_up,
	bool button_right,
	bool button_down,
	bool button_left,
	float widget_size,
	float button_radius,
	const uint32 background_color
)
{
	ImDrawList* draw_list = ImGui::GetWindowDrawList();
	const ImVec2 cursor	  = ImGui::GetCursorScreenPos();
	const ImVec2 center	  = ImVec2 { cursor.x + widget_size * 0.5f, cursor.y + widget_size * 0.5f };

	const bool buttons[4] { button_up, button_right, button_down, button_left };
	uint32 colors[4] {
		IM_COL32( 200, 200, 0, 255 ),
		IM_COL32( 200, 0, 0, 255 ),
		IM_COL32( 0, 200, 0, 255 ),
		IM_COL32( 0, 0, 200, 255 ),
	};
	constexpr ImVec2 directions[4] {
		ImVec2 { 0.0f, -1.0f },
		ImVec2 { 1.0f, 0.0f },
		ImVec2 { 0.0f, 1.0f },
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

void ImGui::InputVisualizer::DpadButtons(
	bool button_up,
	bool button_right,
	bool button_down,
	bool button_left,
	float widget_size,
	float button_size,
	const uint32 background_color
)
{
	ImDrawList* draw_list = ImGui::GetWindowDrawList();
	const ImVec2 cursor	  = ImGui::GetCursorScreenPos();
	const ImVec2 center	  = ImVec2 { cursor.x + widget_size * 0.5f, cursor.y + widget_size * 0.5f };

	const bool buttons[4] { button_up, button_right, button_down, button_left };
	constexpr ImVec2 directions[4] {
		ImVec2 { 0.0f, -1.0f },
		ImVec2 { 1.0f, 0.0f },
		ImVec2 { 0.0f, 1.0f },
		ImVec2 { -1.0f, 0.0f },
	};

	constexpr uint32 PRESSED_COLOR = IM_COL32( 255, 255, 255, 255 );
	const float offset			   = widget_size * 0.5f;
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

void ImGui::InputVisualizer::Shoulder(
	bool left_trigger,
	bool left_shoulder,
	bool right_trigger,
	bool right_shoulder,
	float widget_size,
	float button_size,
	const uint32 background_color
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
		ImVec2 {
			cursor.x + widget_size * 0.5f + widget_size * 0.15f,
			cursor.y + widget_size * 0.6f
		},
		left_trigger ? PRESSED_COLOR : background_color
	);

	ImGui::Dummy( ImVec2( widget_size, widget_size ) );
}
