#pragma once

#include <imgui.h>
#include <misc/cpp/imgui_stdlib.h>

namespace ImGui::Extra
{
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
}