#pragma once

#include <imgui.h>

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
}