#pragma once

#include <suprengine/entity.h>
#include <suprengine/components/transform.h>

namespace suprengine
{
	class MouseFollower : Component
	{
	public:
		MouseFollower( int priority_order = 0 ) : Component( priority_order ) {}

		void update( float dt ) override
		{
			int x, y;
			SDL_GetMouseState( &x, &y );

			transform->set_location( Vec2 { (float) x, (float) y } );
			//printf( "%f %f\n", owner->transform->pos.x, owner->transform->pos.y );
		}
	};
}