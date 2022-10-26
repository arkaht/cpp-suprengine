#pragma once
#include "transform2.fwd.h"
#include "../component.h"

#include "suprengine/vec2.h"

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
		float rotation { 0.0f };

		void update( float dt ) override
		{
			pos += Vec2 { 100.0f * dt, 50.0f * dt };
			printf( "x:%f; y:%f\n", pos.x, pos.y );
		}
	};
}