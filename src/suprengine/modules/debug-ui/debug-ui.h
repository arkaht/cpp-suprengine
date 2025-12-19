#pragma once

namespace suprengine
{
	class DebugUI
	{
	public:
		static void listen();
		static void populate();

	private:
		DebugUI() = default;

	private:
		static DebugUI& instance();

	private:
		bool _is_opened = false;
		bool _show_imgui_demo = false;
		bool _show_implot_demo = false;
		bool _show_profiler = false;
	};
}
