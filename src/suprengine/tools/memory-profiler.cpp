#include "memory-profiler.h"

using namespace suprengine;

std::size_t MemoryProfiler::new_total_calls = 0;
std::size_t MemoryProfiler::delete_total_calls = 0;
std::size_t MemoryProfiler::current_allocated_bytes = 0;
std::size_t MemoryProfiler::total_allocated_bytes = 0;

void* operator new( std::size_t bytes ) throw( std::bad_alloc )
{
	if ( bytes == 0 )
	{
		bytes = 1;
	}

	MemoryProfiler::new_total_calls++;

	while ( true )
	{
		void* pointer = std::malloc( bytes );
		if ( pointer != nullptr )
		{
			MemoryProfiler::current_allocated_bytes += bytes;
			MemoryProfiler::total_allocated_bytes += bytes;
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

	MemoryProfiler::delete_total_calls++;
	MemoryProfiler::current_allocated_bytes -= bytes;

	std::free( pointer );
}