#pragma once

#include <imgui.h>
#include <misc/cpp/imgui_stdlib.h>

#include <vector>

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
}