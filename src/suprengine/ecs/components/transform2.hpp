#pragma once
#include "transform2.fwd.h"

#include <suprengine/ecs/component.h>
#include <suprengine/rect.h>

namespace suprengine
{
	class Transform2 : public Component
	{
	public:
		Transform2( Entity* owner, int priority_order = 0 )
			: Component( owner, priority_order ) {}

		Transform2() = delete;
		Transform2( const Transform2& ) = delete;
		Transform2& operator=( const Transform2& ) = delete;

		Vec2 pos { Vec2::zero }, size { Vec2::one };
		double rotation { 0.0f };  //  rotation in degrees

		Rect get_rect( const Rect& rect )
		{
			return { 
				pos.x + rect.x * size.x, 
				pos.y + rect.y * size.y, 
				rect.w * size.x, 
				rect.h * size.y 
			};
		}
	};
}