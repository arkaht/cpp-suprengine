#include "profiler.h"

#include <suprengine/engine.h>

#include <implot.h>
#include <implot_internal.h>

using namespace suprengine;

//	Frame-per-second target to profile data on the timeline
constexpr int TIMELINE_FPS_TARGET = 60;
//	In seconds, the maximum X-axis value
constexpr float TIMELINE_MAX_TIME = 60.0f;
//	Amount of frames to record for the timeline; by default, allows for 60 seconds at 60 FPS
constexpr int TIMELINE_DATA_SIZE = static_cast<int>( TIMELINE_MAX_TIME * TIMELINE_FPS_TARGET );
constexpr double TIMELINE_BAR_SIZE = 1.0 / TIMELINE_MAX_TIME;

ProfileTimer::ProfileTimer( const char* name, bool is_running, bool should_log )
	: name( name ), _should_log( should_log )
{
	if ( is_running )
	{
		start();
	}
}

ProfileTimer::~ProfileTimer()
{
	stop();
}

void ProfileTimer::start()
{
	_is_running = true;
	_start_timepoint = chrono::high_resolution_clock::now();
}

void ProfileTimer::stop()
{
	if ( !is_running() ) return;

	_total_time = get_time();

	if ( _should_log )
	{
		//	Add result to profiler
		Profiler* profiler = Engine::instance().get_profiler();
		profiler->add_result( name, _total_time );
	}

	_is_running = false;
}

void ProfileTimer::clear()
{
	_total_time = 0.0f;

	if ( is_running() )
	{
		start();
	}
}

bool ProfileTimer::is_running() const
{
	return _is_running;
}

float ProfileTimer::get_time() const
{
	if ( !is_running() ) return _total_time;

	//	Compute start and end times
	TimePoint end_timepoint = chrono::high_resolution_clock::now();
	long long start_ms = chrono::time_point_cast<chrono::microseconds>( _start_timepoint ).time_since_epoch().count();
	long long end_ms = chrono::time_point_cast<chrono::microseconds>( end_timepoint ).time_since_epoch().count();

	//	Compute time difference of timepoints in milliseconds
	float time = ( end_ms - start_ms ) * 0.001f;
	return _total_time + time;
}


Profiler::Profiler( bool is_running )
	: _timer( "Profiler", is_running, /* should_log */ false )
{}

void Profiler::add_result( const char* name, float time )
{
	if ( !is_profiling() ) return;

	auto itr = _results.find( name );
	if ( itr == _results.end() )
	{
		itr = _results.emplace( name, ProfileResult {} ).first;
	}

	ProfileResult& result = itr->second;
	result.time = time;
	result.max_time = std::max( result.max_time, time );
	result.min_time = std::min( result.min_time, time );
	result.total_time += time;
	result.non_consumed_time += time;
	result.total_calls++;
	result.non_consumed_calls++;
}

void Profiler::consume_results()
{
	for ( auto& pair : _results )
	{
		ProfileResult& result = pair.second;
		result.non_consumed_time = 0.0f;
		result.non_consumed_calls = 0;
	}
}

void Profiler::clear()
{
	_results.clear();
	_timer.clear();
	_timelines.clear();
}

void Profiler::start()
{
	_timer.start();
}

void Profiler::stop()
{
	_timer.stop();
}

void Profiler::update()
{
	if ( !is_profiling() ) return;

	auto& engine = Engine::instance();
	auto updater = engine.get_updater();

	const float profile_time = get_profile_time() * 0.001f;
	for ( auto& pair : _results )
	{
		auto itr = _timelines.find( pair.first );
		if ( itr == _timelines.end() )
		{
			itr = _timelines.emplace( pair.first, ImGui::Extra::ScrollingBuffer<Vec2>( TIMELINE_DATA_SIZE ) ).first;
		}

		itr->second.add_point(
			Vec2 {
				profile_time,
				pair.second.non_consumed_time
				//updater->get_unscaled_delta_time() * 1000.0f
			}
		);
	}

	consume_results();
}

struct TimelineImData
{
	bool is_hidden = false;
	ImVec4 color {};
};

void Profiler::populate_imgui()
{
	const auto& results = get_results();
	const float profile_time_ms = get_profile_time();
	const float profile_time_seconds = profile_time_ms * 0.001f;

	auto& engine = Engine::instance();
	auto renderer = engine.get_render_batch();

	ImGui::SeparatorText( "Metrics" );
	ImGui::Text( "Viewport Renderers Count: %d", renderer->get_renderers_count( RenderPhase::Viewport ) );
	ImGui::Text( "World Renderers Count: %d", renderer->get_renderers_count( RenderPhase::World ) );

	//	TODO: Add sorting
	ImGuiTableFlags table_flags = ImGuiTableFlags_ScrollX | ImGuiTableFlags_ScrollY;
	table_flags |= ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg;
	table_flags |= ImGuiTableFlags_Resizable | ImGuiTableFlags_Hideable;
	//table_flags |= ImGuiTableFlags_SizingFixedFit;

	ImGui::SeparatorText( "Results" );
	const char* column_names[] {
		"Name",
		"Last Time", "Min. Time", "Avg. Time", "Max. Time",
		"Frame Time", "Frame Calls",
		"Total Calls",
		"Usage",
	};
	constexpr int COLUMNS_AMOUNT = IM_ARRAYSIZE( column_names );
	constexpr ImVec2 TABLE_SIZE { 0.0f, 250.0f };
	if ( ImGui::BeginTable( "suprengine_profiler_results", COLUMNS_AMOUNT, table_flags, TABLE_SIZE ) )
	{
		//	Setup first column
		ImGui::TableSetupColumn( column_names[0], ImGuiTableColumnFlags_NoHide | ImGuiTableColumnFlags_NoReorder );

		//	Setup remaining columns
		ImGuiTableColumnFlags column_flags = ImGuiTableColumnFlags_WidthFixed;
		for ( int i = 1; i < COLUMNS_AMOUNT; i++ )
		{
			ImGui::TableSetupColumn( column_names[i], column_flags );
		}

		//	Freeze headers and first column
		ImGui::TableSetupScrollFreeze( 1, 1 );

		//	Draw angled headers if needed
		if ( column_flags & ImGuiTableColumnFlags_AngledHeader )
		{
			ImGui::TableAngledHeadersRow();
		}
		ImGui::TableHeadersRow();

		//	Draw results
		int row_id = 0;
		for ( const auto& result_pair : results )
		{
			const char* name = result_pair.first;
			const ProfileResult& result = result_pair.second;

			ImGui::PushID( row_id );
			ImGui::TableNextRow( ImGuiTableRowFlags_None );

			//	Name
			ImGui::TableNextColumn();
			ImGui::TextUnformatted( name );

			//	Last Time
			ImGui::TableNextColumn();
			ImGui::Text( "%.3fms", result.time );

			//	Min. Time
			ImGui::TableNextColumn();
			ImGui::Text( "%.3fms", result.min_time );

			//	Avr. Time
			ImGui::TableNextColumn();
			ImGui::Text( "%.3fms", result.total_time / result.total_calls );

			//	Max. Time
			ImGui::TableNextColumn();
			ImGui::Text( "%.3fms", result.max_time );

			//	Frame Time
			ImGui::TableNextColumn();
			ImGui::Text( "%.3fms", result.non_consumed_time );

			//	Frame Calls
			ImGui::TableNextColumn();
			ImGui::Text( "%d", result.non_consumed_calls );

			//	Total Calls
			ImGui::TableNextColumn();
			ImGui::Text( "%d", result.total_calls );

			//	Usage
			ImGui::TableNextColumn();
			ImGui::Text( "%.1f%%", result.total_time / profile_time_ms * 100.0f );

			ImGui::PopID();
		}

		ImGui::EndTable();
	}

	ImGui::SeparatorText( "Graphs" );
	if ( ImPlot::BeginPlot( "Timelines", ImVec2 { -1.0f, 250.0f } ) )
	{
		ImPlot::SetupAxes( "Profile Time (s)", "Result Time (ms)", ImPlotAxisFlags_AutoFit );
		ImPlot::SetupAxesLimits( 0, TIMELINE_MAX_TIME, 0, 50 );
		ImPlot::SetupAxisLimitsConstraints(
			ImAxis_X1,
			math::max( 0.0f, profile_time_seconds - TIMELINE_MAX_TIME ),
			math::max( static_cast<float>( TIMELINE_MAX_TIME ), profile_time_seconds )
		);
		ImPlot::SetupAxisLimitsConstraints( ImAxis_Y1, 0.0, 200.0 );
		ImPlot::SetupLegend( ImPlotLocation_West, ImPlotLegendFlags_Outside );

		//	Draw timelines
		std::vector<TimelineImData> timelines_data {};
		timelines_data.reserve( _timelines.size() );
		for ( auto& pair : _timelines )
		{
			const char* name = pair.first;
			auto& timeline = pair.second;

			int timeline_size = static_cast<int>( timeline.data.size() );
			if ( timeline_size == 0 ) return;

			ImPlot::PlotBars(
				name,
				&timeline.data[0].x, &timeline.data[0].y,
				timeline_size,
				TIMELINE_BAR_SIZE,
				ImPlotBarsFlags_None,
				timeline.offset, sizeof( Vec2 )
			);

			//	Fill timeline data for later use
			ImPlotContext* plot_context = ImPlot::GetCurrentContext();
			TimelineImData& data = timelines_data.emplace_back( TimelineImData {} );
			data.color = ImPlot::GetLastItemColor();
			data.is_hidden = !plot_context->PreviousItem->Show;
		}

		//	Inspect timeline under mouse position
		if ( ImPlot::IsPlotHovered() && ImGui::GetIO().KeyCtrl )
		{
			ImPlotPoint mouse_pos = ImPlot::GetPlotMousePos();

			const char* hovered_timeline_name = nullptr;
			double hovered_timeline_y = mouse_pos.y;
			int hovered_timeline_id = -1;

			//	Find the hovered timeline
			int id = -1;
			for ( auto& pair : _timelines )
			{
				id++;

				//	Skip empty timelines
				auto& timeline = pair.second;
				if ( timeline.data.empty() ) continue;

				//	Skip hidden timelines (by user)
				TimelineImData& data = timelines_data.at( id );
				if ( data.is_hidden ) continue;

				for ( const Vec2& pos : timeline.data )
				{
					//	Skip all positions before the mouse X-pos
					if ( pos.x < mouse_pos.x - TIMELINE_BAR_SIZE * 0.5 ) continue;
					//	Then, stop if Y-pos is not high enough to have the mouse Y-pos under it
					if ( pos.y < mouse_pos.y ) break;

					//	We have a potential winner, let's see the other timelines
					hovered_timeline_name = pair.first;
					hovered_timeline_y = pos.y;
					hovered_timeline_id = id;
					break;
				}
			}

			constexpr ImVec2 ANNOTATION_OFFSET { 0.0f, -15.0f };
			if ( hovered_timeline_id != -1 )
			{
				//	Draw annotation about the hovered timeline
				TimelineImData& data = timelines_data.at( hovered_timeline_id );
				ImPlot::Annotation(
					mouse_pos.x, hovered_timeline_y,
					data.color,
					ANNOTATION_OFFSET,
					/* clamp */ true,
					"%s - %.3fms",
					hovered_timeline_name,
					hovered_timeline_y
				);
			}
			else
			{
				constexpr ImVec4 SELECTOR_COLOR { 0.5f, 0.5f, 0.5f, 1.0f };
				ImPlot::Annotation(
					mouse_pos.x, mouse_pos.y,
					SELECTOR_COLOR,
					ANNOTATION_OFFSET,
					/* clamp */ true,
					"None"
				);
			}
		}

		//	Draw time target
		constexpr ImVec4 TIME_TARGET_COLOR { 1.0f, 0.3f, 0.3f, 0.75f };
		double time_target = 1.0 / TIMELINE_FPS_TARGET * 1000.0;
		ImPlot::TagY( time_target, TIME_TARGET_COLOR, "Target" );
		ImPlot::DragLineY( 0, &time_target, TIME_TARGET_COLOR, 2.0f, ImPlotDragToolFlags_NoInputs );

		ImPlot::EndPlot();
	}

	const char* toggle_text = is_profiling() ? "Stop" : "Start";
	if ( ImGui::Button( toggle_text ) )
	{
		if ( is_profiling() )
		{
			stop();
		}
		else
		{
			start();
		}
	}
	ImGui::SameLine();
	if ( ImGui::Button( "Clear" ) )
	{
		clear();
	}
	ImGui::SameLine();
	ImGui::Text( "Profile Time: %.1fs", profile_time_seconds );
}

bool Profiler::is_profiling() const
{
	return _timer.is_running();
}

float Profiler::get_profile_time() const
{
	return _timer.get_time();
}

const Profiler::ProfileResultsMap& Profiler::get_results() const
{
	return _results;
}