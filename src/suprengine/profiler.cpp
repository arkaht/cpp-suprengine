#include "profiler.h"

#include <suprengine/engine.h>

using namespace suprengine;

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

	//	Compute time difference of time points
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
	result.total_calls++;
}

{
	_results.clear();

	//	Reset timer
	if ( is_profiling() )
	{
		_timer.start();
	}
}

void Profiler::clear()
{
	_results.clear();
	_timer.clear();
}

void Profiler::start()
{
	_timer.start();
}

void Profiler::stop()
{
	_timer.stop();
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