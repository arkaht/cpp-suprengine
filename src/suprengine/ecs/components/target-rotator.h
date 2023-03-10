#pragma once
#include "transform.h"

namespace suprengine
{
	class TargetRotator : public Component
	{
	private:
	public:
		Transform* target { nullptr };

		TargetRotator( Entity* owner, Transform* target )
			: target( target ), Component( owner ) {}

		void update( float dt ) override
		{
			transform->look_at( target->location );
		}
	};
}