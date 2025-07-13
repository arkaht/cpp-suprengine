#pragma once

#include <imgui.h>
#include <misc/cpp/imgui_stdlib.h>

#include <vector>

#include <suprengine/math/vec2.h>
#include <suprengine/utils/usings.h>

namespace ImGui::Extra
{
	/*
	 * Struct storing points in a way that allows to create a scrolling histogram.
	 * This code is took from implot_demo.cpp
	 */
	template <typename DataType>
	struct ScrollingBuffer
	{
		ScrollingBuffer( int max_size )
			: max_size( max_size )
		{
			data.reserve( max_size );
		}

		void add_point( const DataType& point )
		{
			if ( data.size() < max_size )
			{
				data.emplace_back( point );
			}
			else
			{
				data[offset] = point;
				offset = ( offset + 1 ) % max_size;
			}
		}

		void clear()
		{
			data.clear();
			offset = 0;
		}

		int max_size = 100;
		int offset = 0;
		std::vector<DataType> data {};
	};

	static void ColoredProgressBar(
		float fraction,
		ImVec4 color,
		const ImVec2& size_arg = ImVec2( -FLT_MIN, 0 ),
		const char* overlay = NULL
	)
	{
		ImGui::PushStyleColor( ImGuiCol_PlotHistogram, color );
		ImGui::ProgressBar( fraction, size_arg, overlay );
		ImGui::PopStyleColor();
	}

	static bool SliderPercent(
		const char* label,
		float* v,
		float v_min, float v_max,
		const char* format = "%.0f%%"
	)
	{
		float percent = *v * 100.0f;
		if ( ImGui::SliderFloat( label,	&percent, v_min * 100.0f, v_max * 100.0f, format ) )
		{
			*v = percent / 100.0f;
			return true;
		}

		return false;
	}

	static bool DragPercent(
		const char* label,
		float* v,
		float v_speed,
		float v_min, float v_max,
		const char* format = "%.0f%%"
	)
	{
		float percent = *v * 100.0f;
		if ( ImGui::DragFloat( label, &percent, v_speed * 100.0f, v_min * 100.0f, v_max * 100.0f, format ) )
		{
			*v = percent / 100.0f;
			return true;
		}

		return false;
	}

	static bool ColoredTreeNode(
		const char* label,
		ImGuiTreeNodeFlags flags,
		const ImVec4& color
	)
	{
		int to_pop = 1;
		if ( flags & ImGuiTreeNodeFlags_Framed )
		{
			ImGui::PushStyleColor( ImGuiCol_Header, color );
			ImGui::PushStyleColor( ImGuiCol_HeaderHovered, color );
			ImGui::PushStyleColor( ImGuiCol_HeaderActive, color );
			to_pop = 3;
		}
		else
		{
			ImGui::PushStyleColor( ImGuiCol_Text, color );
		}

		bool is_pressed = ImGui::TreeNodeEx( label, flags );
		ImGui::PopStyleColor( to_pop );

		return is_pressed;
	}

	constexpr float GAMEPAD_WIDGET_SIZE = 48.0f;
	constexpr float JOYSTICK_WIDGET_RADIUS = GAMEPAD_WIDGET_SIZE / 2.0f;
	constexpr float JOYSTICK_RADIUS = 4.0f;
	constexpr float FACE_BUTTONS_RADIUS = 8.0f;
	constexpr uint32 GAMEPAD_WIDGETS_BACKGROUND_COLOR = IM_COL32( 0, 0, 0, 155 );

	static void Joystick(
		suprengine::Vec2 joystick,
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
		const ImVec2 center = ImVec2( cursor.x + widget_radius, cursor.y + widget_radius );
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

		// Draw deadzone
		constexpr uint32 DEADZONE_COLOR = IM_COL32( 255, 0, 0, 255 );
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
			bool has_exceeded = false;
			if ( amplitude > 1.0f )
			{
				joystick /= amplitude;

				has_exceeded = true;
				amplitude = 1.0f;
			}

			draw_list->AddCircle(
				center,
				widget_radius * amplitude,
				has_exceeded ? DEADZONE_COLOR : amplitude_color
			);
		}

		// Draw joystick
		const ImVec2 joystick_center {
			joystick.x * ( widget_radius - joystick_radius ) + cursor.x + widget_radius,
			-joystick.y * ( widget_radius - joystick_radius ) + cursor.y + widget_radius,
		};
		draw_list->AddCircleFilled(
			joystick_center,
			joystick_radius,
			joystick_color
		);

		if ( is_pressed )
		{
			draw_list->AddCircle( joystick_center, joystick_radius * 1.5f, joystick_color );
		}

		ImGui::Dummy( ImVec2( widget_radius * 2.0f, widget_radius * 2.0f ) );
	};

	static void FaceButtons(
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

	static void DpadButtons(
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

	static void Shoulder(
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
}