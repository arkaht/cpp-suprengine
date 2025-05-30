#pragma once

#include <suprengine/tools/memory-profiler.h>

namespace suprengine
{
	class IGame;
	class Engine;

	class Scene
	{
	public:
		Scene();
		virtual ~Scene() {}

		virtual void init() {}
		virtual void update( float dt ) {}

		IGame* get_game() const { return _game; }

	public:
	#ifdef ENABLE_MEMORY_PROFILER
		static void* operator new( std::size_t bytes )
		{
			return MemoryProfiler::allocate( "Scene", bytes );
		}
	#endif
	
	protected:
		IGame* _game { nullptr };
	};
}