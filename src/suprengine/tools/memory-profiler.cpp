#include "memory-profiler.h"

#include <vector>

using namespace suprengine;

static MemoryGlobalProfileResult global_result {};
static MemoryProfiler::LocalResultsMap local_results {};
static std::vector<const char*> profiled_names {};


ScopedMemoryProfile::ScopedMemoryProfile( const char* name )
	: name( name )
{
	MemoryProfiler::start_local_profiling( name );
}

ScopedMemoryProfile::~ScopedMemoryProfile()
{
	MemoryProfiler::stop_local_profiling( name );
}


void* operator new( std::size_t bytes ) throw( std::bad_alloc )
{
	if ( bytes == 0 )
	{
		bytes = 1;
	}

	//	Register into global result
	global_result.new_total_calls++;
	global_result.new_max_bytes = std::max( global_result.new_max_bytes, bytes );
	global_result.new_min_bytes = std::min( global_result.new_min_bytes, bytes );
	global_result.current_allocated_bytes += bytes;
	global_result.total_allocated_bytes += bytes;

	//	Register into local results
	for ( const char* name : profiled_names )
	{
		auto itr = local_results.find( name );

		MemoryLocalProfileResult& result = itr->second;
		result.new_total_calls++;
		result.new_max_bytes = std::max( result.new_max_bytes, bytes );
		result.new_min_bytes = std::min( result.new_min_bytes, bytes );
		result.non_consumed_allocated_bytes += bytes;
		result.total_allocated_bytes += bytes;
	}

	//	Basic implementation from Scott Meyers' book "Effective C++"
	while ( true )
	{
		void* pointer = std::malloc( bytes );
		if ( pointer != nullptr )
		{
			return pointer;
		}
		
		const std::new_handler handler = std::get_new_handler();
		if ( handler )
		{
			( *handler )();
			continue;
		}

		throw std::bad_alloc();
	}
}

void operator delete( void* pointer, std::size_t bytes ) throw()
{
	if ( pointer == nullptr ) return;

	//	Register into global result
	global_result.delete_total_calls++;
	global_result.current_allocated_bytes -= bytes;

	//	Register into local results
	for ( const char* name : profiled_names )
	{
		auto itr = local_results.find( name );

		MemoryLocalProfileResult& result = itr->second;
		result.delete_total_calls++;
	}

	std::free( pointer );
}

void MemoryProfiler::start_local_profiling( const char* name )
{
	auto itr = local_results.find( name );
	if ( itr == local_results.end() )
	{
		itr = local_results.emplace( name, MemoryLocalProfileResult {} ).first;
	}

	itr->second.total_profile_calls++;
	itr->second.non_consumed_allocated_bytes = 0;

	profiled_names.push_back( name );
}

void MemoryProfiler::stop_local_profiling( const char* name )
{
	auto itr = std::find( profiled_names.begin(), profiled_names.end(), name );
	if ( itr == profiled_names.end() ) return;

	profiled_names.erase( itr );
}

const MemoryGlobalProfileResult& MemoryProfiler::get_global_result()
{
	return global_result;
}

const MemoryProfiler::LocalResultsMap& MemoryProfiler::get_local_results()
{
	return local_results;
}
