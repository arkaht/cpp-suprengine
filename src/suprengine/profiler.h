#pragma once

#include <suprengine/usings.h>
#include <suprengine/math.h>

#include <map>
#include <chrono>

#define PROFILE_SCOPE( name ) ProfileTimer _profile_timer##__LINE__( name )

namespace suprengine
{
	namespace chrono = std::chrono;

	struct ProfileResult
	{
		float time = 0.0f;
		float min_time = math::PLUS_INFINITY;
		float max_time = math::NEG_INFINITY;
		float total_time = 0.0f;
		float non_consumed_time = 0.0f;
		uint32 total_calls = 0;
		uint32 non_consumed_calls = 0;
	};

	class ProfileTimer
	{
	public:
		using TimePoint = chrono::steady_clock::time_point;

	public:
		ProfileTimer( const char* name, bool is_running = true, bool should_log = true );
		~ProfileTimer();

		void start();
		void stop();
		void clear();

		bool is_running() const;
		float get_time() const;

	public:
		const char* name;

	private:
		TimePoint _start_timepoint;

		float _total_time = 0.0f;

		bool _is_running = false;
		bool _should_log = true;
	};

	class Profiler
	{
	public:
		using ProfileResultsMap = std::map<const char*, ProfileResult>;

	public:
		Profiler( bool is_running = true );

		void add_result( const char* name, float time );
		void consume_results();
		void clear();

		void start();
		void stop();

		void populate_imgui();

		bool is_profiling() const;
		/*
		 * Returns the current profiling time in milliseconds.
		 */
		float get_profile_time() const;

		const ProfileResultsMap& get_results() const;

	private:
		ProfileResultsMap _results {};

		/*
		 * Timer to compute the total profile time
		 */
		ProfileTimer _timer;
	};
}