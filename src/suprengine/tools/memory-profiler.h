#pragma once

#include <suprengine/utils/usings.h>

#include <map>

#define MEMORY_SCOPE( name ) ScopedMemoryProfile _scoped_memory_profile##__LINE__( name )

namespace suprengine
{
	struct MemoryCategoryProfileResult
	{
		std::size_t total_allocated_bytes = 0;
		std::size_t current_allocated_bytes = 0;

		std::size_t total_instances = 0;
		std::size_t current_instances = 0;
	};

	struct MemoryLocalProfileResult
	{
		std::size_t total_allocated_bytes = 0;
		std::size_t non_consumed_allocated_bytes = 0;

		std::size_t new_min_bytes = std::numeric_limits<std::size_t>::max();
		std::size_t new_max_bytes = 0;

		std::size_t total_instances = 0;
		std::size_t delete_total_calls = 0;

		std::size_t total_profile_calls = 0;
	};

	struct MemoryGlobalProfileResult
	{
		std::size_t total_instances = 0;
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
		using CategoryResultsMap = std::map<const char*, MemoryCategoryProfileResult>;

	public:
		template <typename T, typename ...Args>
		static T* allocate( const char* category, Args ...args )
		{
			_set_current_category( category );
			T* pointer = new T( args... );
			_unset_current_category();

			return pointer;
		}

		static void* allocate( const char* category, std::size_t bytes );

		static void start_local_profiling( const char* name );
		static void stop_local_profiling( const char* name );

		static const MemoryGlobalProfileResult& get_global_result();
		static const LocalResultsMap& get_local_results();

		static const CategoryResultsMap& get_custom_categories_results();
		static const MemoryCategoryProfileResult& get_other_category_result();

	private:
		static void _set_current_category( const char* category );
		static void _unset_current_category();
	};
}