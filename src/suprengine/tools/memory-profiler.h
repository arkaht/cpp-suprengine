#pragma once

#include <suprengine/utils/usings.h>

namespace suprengine
{
	class MemoryProfiler
	{
	public:
		static std::size_t new_total_calls;
		static std::size_t delete_total_calls;
		static std::size_t total_allocated_bytes;
		static std::size_t current_allocated_bytes;
	};
}