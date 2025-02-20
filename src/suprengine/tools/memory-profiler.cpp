#include "memory-profiler.h"

#include <suprengine/core/engine.h>

#include <suprengine/utils/assert.h>

#include <vector>
#include <unordered_map>

using namespace suprengine;

static MemoryGlobalProfileResult global_result {};
static MemoryProfiler::LocalResultsMap local_results {};
static std::vector<const char*> profiled_names {};

static const char* current_category = nullptr;
static MemoryCategoryProfileResult other_category_result {};
static MemoryProfiler::CategoryResultsMap category_results {};

static std::unordered_map<void*, const char*> categorized_addresses {};


ScopedMemoryProfile::ScopedMemoryProfile( const char* name )
	: name( name )
{
	MemoryProfiler::start_local_profiling( name );
}

ScopedMemoryProfile::~ScopedMemoryProfile()
{
	MemoryProfiler::stop_local_profiling( name );
}


void* operator new( std::size_t bytes )
{
	if ( bytes == 0 )
	{
		bytes = 1;
	}

	//	Register into global result
	global_result.total_instances++;
	global_result.new_max_bytes = std::max( global_result.new_max_bytes, bytes );
	global_result.new_min_bytes = std::min( global_result.new_min_bytes, bytes );
	global_result.current_allocated_bytes += bytes;
	global_result.total_allocated_bytes += bytes;

	//	Register into local results
	for ( const char* name : profiled_names )
	{
		const MemoryProfiler::LocalResultsMap::iterator itr = local_results.find( name );

		MemoryLocalProfileResult& result = itr->second;
		result.total_instances++;
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
			//	Register into current category result
			if ( current_category == nullptr )
			{
				other_category_result.total_instances++;
				other_category_result.current_instances++;
				other_category_result.current_allocated_bytes += bytes;
				other_category_result.total_allocated_bytes += bytes;
			}
			else
			{
				const MemoryProfiler::CategoryResultsMap::iterator itr = category_results.find( current_category );

				MemoryCategoryProfileResult& result = itr->second;
				result.total_instances++;
				result.current_instances++;
				result.current_allocated_bytes += bytes;
				result.total_allocated_bytes += bytes;

				current_category = nullptr;
				categorized_addresses.emplace( pointer, itr->first );
				current_category = itr->first;
			}

			return pointer;
		}

		const std::new_handler handler = std::get_new_handler();
		if ( handler != nullptr )
		{
			( *handler )( );
			continue;
		}

		throw std::bad_alloc();
	}
}

void operator delete( void* pointer, std::size_t bytes )
{
	if ( pointer == nullptr ) return;

	//	Avoid crashing when ending the program because 'categorized_addresses' has already been released.
	//	Not that pretty but it works.
	bool should_profile = Engine::instance().is_running();
	if ( should_profile )
	{
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

		//	Register into category result
		const auto itr = categorized_addresses.find( pointer );
		if ( itr != categorized_addresses.end() )
		{
			const char* category = itr->second;

			const MemoryProfiler::CategoryResultsMap::iterator category_result_itr = category_results.find( category );
			//	TODO: Check whether it's a good idea to put an assert in a critical operator.
			ASSERT( category_result_itr != category_results.end() );

			MemoryCategoryProfileResult& result = category_result_itr->second;
			result.current_instances--;
			result.current_allocated_bytes -= bytes;

			categorized_addresses.erase( itr );
		}
		else
		{
			other_category_result.current_instances--;
			other_category_result.current_allocated_bytes -= bytes;
		}
	}

	std::free( pointer );
}

void* MemoryProfiler::allocate( const char* category, std::size_t bytes )
{
	_set_current_category( category );
	void* pointer = ::operator new( bytes );
	_unset_current_category();

	return pointer;
}

void MemoryProfiler::start_local_profiling( const char* name )
{
	LocalResultsMap::iterator itr = local_results.find( name );
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

const MemoryProfiler::CategoryResultsMap& MemoryProfiler::get_custom_categories_results()
{
	return category_results;
}

const MemoryCategoryProfileResult& MemoryProfiler::get_other_category_result()
{
	return other_category_result;
}


void MemoryProfiler::_set_current_category( const char* category )
{
	//	Create category if it doesn't exist yet
	const CategoryResultsMap::const_iterator itr = category_results.find( category );
	if ( itr == category_results.end() )
	{
		category_results.emplace( category, MemoryCategoryProfileResult {} ).first;
	}

	current_category = category;
}

void MemoryProfiler::_unset_current_category()
{
	current_category = nullptr;
}