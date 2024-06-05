#pragma once
#include "transform.h"

namespace suprengine
{
	class TargetRotator : public Component
	{
	public:
		TargetRotator( shared_ptr<Entity> owner, shared_ptr<Transform> target )
			: target( target ), Component( owner ) {}

		void update( float dt ) override
		{
			transform->look_at( target->location );
		}

	public:
		shared_ptr<Transform> target;
	};
}