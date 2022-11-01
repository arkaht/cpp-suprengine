#pragma once

#include "collider.h"
#include <suprengine/ecs/entity.h>
#include <suprengine/ecs/components/transform2.hpp>

namespace suprengine
{
	class RectCollider : public Collider
	{
	public:
		Rect shape;

		RectCollider( Entity* owner, Rect shape, int priority_order = 0 )
			: shape( shape ), Collider( owner, priority_order ) {}

		bool intersects( Collider* c ) override
		{
			Rect abs_shape = owner->transform->get_rect( shape );

			//  rect-rect collision
			if ( auto rc = dynamic_cast<RectCollider*>( c ) )
			{
				Rect abs_rc_shape = rc->owner->transform->get_rect( shape );
				return abs_shape.x < abs_rc_shape.x + abs_rc_shape.w && abs_rc_shape.x < abs_shape.x + abs_shape.w
					&& abs_shape.y < abs_rc_shape.y + abs_rc_shape.h && abs_rc_shape.y < abs_shape.y + abs_shape.h;
			}
		}

		void debug_render( RenderBatch* render_batch ) override
		{
			render_batch->draw_rect( 
				DrawType::FILL, 
				owner->transform->get_rect( shape ), 
				COLOR 
			);
		}
	};
}