#pragma once
#include "transform.h"

namespace suprengine
{
	class TargetRotator : public Component
	{
	public:
		TargetRotator( SharedPtr<Entity> owner, SharedPtr<Transform> target )
			: target( target ), Component( owner ) {}

		void update( float dt ) override
		{
			transform->look_at( target->location );
		}

	public:
		SharedPtr<Transform> target;
	};
}