#pragma once

#include <suprengine/utils/usings.h>

#include <map>

#define MEMORY_SCOPE( name ) ScopedMemoryProfile _scoped_memory_profile##__LINE__( name )

namespace suprengine
{
	struct MemoryLocalProfileResult
	{
		std::size_t total_allocated_bytes = 0;
		std::size_t non_consumed_allocated_bytes = 0;

		std::size_t new_min_bytes = std::numeric_limits<std::size_t>::max();
		std::size_t new_max_bytes = 0;

		std::size_t new_total_calls = 0;
		std::size_t delete_total_calls = 0;

		std::size_t total_profile_calls = 0;
	};

	struct MemoryGlobalProfileResult
	{
		std::size_t new_total_calls = 0;
		std::size_t delete_total_calls = 0;

		std::size_t new_min_bytes = std::numeric_limits<std::size_t>::max();
		std::size_t new_max_bytes = 0;

		std::size_t total_allocated_bytes = 0;
		std::size_t current_allocated_bytes = 0;
	};

	class ScopedMemoryProfile
	{
	public:
		ScopedMemoryProfile( const char* name );
		~ScopedMemoryProfile();

	public:
		const char* name = nullptr;
	};

	class MemoryProfiler
	{
	public:
		using LocalResultsMap = std::map<const char*, MemoryLocalProfileResult>;

	public:
		static void start_local_profiling( const char* name );
		static void stop_local_profiling( const char* name );

		static const MemoryGlobalProfileResult& get_global_result();
		static const LocalResultsMap& get_local_results();
	};
}