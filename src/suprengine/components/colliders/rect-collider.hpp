#pragma once
#include "collider.h"

#include <suprengine/entity.h>
#include <suprengine/components/transform.h>

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
			Rect rect = owner->transform->get_rect( shape );

			//  rect-rect collision
			if ( auto rc = dynamic_cast<RectCollider*>( c ) )
			{
				Rect rc_rect = rc->owner->transform->get_rect( rc->shape );
				return rect.x < rc_rect.x + rc_rect.w && rc_rect.x < rect.x + rect.w
					&& rect.y < rc_rect.y + rc_rect.h && rc_rect.y < rect.y + rect.h;
			}

			return false;
		}
		bool raycast( _RAYCAST_FUNC_PARAMS ) override { return false; }

		void debug_render( RenderBatch* _render_batch ) override
		{
			_render_batch->draw_rect( 
				DrawType::FILL, 
				owner->transform->get_rect( shape ), 
				debug_color
			);
		}
	};
}