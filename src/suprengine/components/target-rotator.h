#pragma once
#include "transform.h"

namespace suprengine
{
	class TargetRotator : public Component
	{
	private:
	public:
		std::shared_ptr<Transform> target;

		TargetRotator( Entity* owner, std::shared_ptr<Transform> target )
			: target( target ), Component( owner ) {}

		void update( float dt ) override
		{
			transform->look_at( target->location );
		}
	};
}