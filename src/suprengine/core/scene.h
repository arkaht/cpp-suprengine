#pragma once

#include <suprengine/tools/memory-profiler.h>

#include "suprengine/components/camera.h"

namespace suprengine
{
	class RenderBatch;
}

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
		virtual void render( RenderBatch* render_batch, const SharedPtr<Camera>& camera );

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