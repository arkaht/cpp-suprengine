#pragma once
#include <suprengine/components/collider.h>
#include <suprengine/components/transform.h>

#include <suprengine/core/entity.h>

namespace suprengine
{
	class RectCollider : public Collider
	{
	public:
		Rect shape;

		RectCollider( SharedPtr<Entity> owner, Rect shape, int priority_order = 0 )
			: shape( shape ), Collider( owner, priority_order ) {}

		bool intersects( SharedPtr<Collider> other ) override
		{
			Rect rect = transform->get_rect( shape );

			//  rect-rect collision
			if ( auto rc = std::dynamic_pointer_cast<RectCollider>( other ) )
			{
				Rect rc_rect = rc->get_owner()->transform->get_rect( rc->shape );
				return rect.x < rc_rect.x + rc_rect.w && rc_rect.x < rect.x + rect.w
					&& rect.y < rc_rect.y + rc_rect.h && rc_rect.y < rect.y + rect.h;
			}

			return false;
		}
		bool raycast( _RAYCAST_FUNC_PARAMS ) override { return false; }

		void debug_render( RenderBatch* _render_batch ) override
		{
			_render_batch->draw_rect( 
				DrawType::Fill, 
				transform->get_rect( shape ), 
				debug_color
			);
		}
	};
}