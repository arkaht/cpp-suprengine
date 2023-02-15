#pragma once

#include <suprengine/ecs/entity.h>
#include <suprengine/ecs/components/transform2.hpp>

namespace suprengine
{
	class MouseFollower : Component
	{
	public:
		MouseFollower( Entity* owner, int priority_order = 0 ) : Component( owner, priority_order ) {}

		void update( float dt ) override
		{
			int x, y;
			SDL_GetMouseState( &x, &y );

			owner->transform->pos = Vec2 { 
				(float) x,
				(float) y 
			};
			//printf( "%f %f\n", owner->transform->pos.x, owner->transform->pos.y );
		}
	};
}