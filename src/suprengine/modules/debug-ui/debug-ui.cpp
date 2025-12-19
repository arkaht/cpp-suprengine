#include "debug-ui.h"

#include "imgui.h"
#include "implot.h"

#include "suprengine/core/engine.h"

using namespace suprengine;

void DebugUI::listen()
{
	Engine& engine = Engine::instance();
	engine.on_imgui_update.listen( &DebugUI::populate );
}

void DebugUI::populate()
{
	DebugUI& instance = DebugUI::instance();

	Engine& engine = Engine::instance();
	InputManager* inputs = engine.get_inputs();

	// Window
	if ( ImGui::Shortcut( ImGuiKey_F2, ImGuiInputFlags_RouteAlways ) )
	{
		instance._is_opened = !instance._is_opened;
		inputs->set_relative_mouse_mode( !instance._is_opened );
	}
	// Profiler
	if ( ImGui::Shortcut( ImGuiKey_F3, ImGuiInputFlags_RouteAlways ) )
	{
		instance._show_profiler = !instance._show_profiler;
	}

	// Show demo windows
	if ( instance._show_imgui_demo )
	{
		ImGui::ShowDemoWindow( &instance._show_imgui_demo );
	}
	if ( instance._show_implot_demo )
	{
		ImPlot::ShowDemoWindow( &instance._show_implot_demo );
	}

	//	Show Profiler window
	if ( instance._show_profiler )
	{
		if ( ImGui::Begin( "Profiler", &instance._show_profiler ) )
		{
			Profiler* profiler = engine.get_profiler();
			profiler->populate_imgui();
		}
		ImGui::End();
	}

	if ( !instance._is_opened )
	{
		return;
	}

	static constexpr ImGuiWindowFlags WINDOW_FLAGS = ImGuiWindowFlags_MenuBar
		| ImGuiWindowFlags_NoSavedSettings; // Disable saved settings to avoid weird sizes caused
											// by window modes

	ImGui::SetNextWindowSize( ImVec2 { 400, 600 }, ImGuiCond_Appearing );

	if ( !ImGui::Begin( "Debug Menu", &instance._is_opened, WINDOW_FLAGS ) )
	{
		ImGui::End();
		return;
	}

	// Populate menu bar
	if ( ImGui::BeginMenuBar() )
	{
		if ( ImGui::BeginMenu( "Engine" ) )
		{
			ImGui::MenuItem( "Profiler", "F3", &instance._show_profiler );

			ImGui::EndMenu();
		}

		if ( ImGui::BeginMenu( "Demos" ) )
		{
			ImGui::MenuItem( "ImGui Demo", nullptr, &instance._show_imgui_demo );
			ImGui::MenuItem( "ImPlot Demo", nullptr, &instance._show_implot_demo );

			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}

	if ( ImGui::BeginTabBar( "menus", ImGuiTabBarFlags_None ) )
	{
		if ( ImGui::BeginTabItem( "Inputs", nullptr, ImGuiTabItemFlags_None ) )
		{
			inputs->populate_imgui();
			ImGui::EndTabItem();
		}

		ImGui::EndTabBar();
	}

	ImGui::End();
}

DebugUI& DebugUI::instance()
{
	static DebugUI instance;
	return instance;
}