#pragma once

#include <suprengine/entity.h>
#include <suprengine/components/transform.h>

namespace puzzle
{
	using namespace suprengine;

	class Projectile : public Component
	{
	public:
		float move_speed = 75.0f;
		float life_time = 2.0f;

		Projectile( Entity* owner ) : Component( owner ) {}

		void update( float dt ) override
		{
			transform->set_location(
				transform->location
				+ transform->get_forward() * move_speed * dt
				//+ Vec3::forward * move_speed * dt 
			);

			life_time -= dt;
			if ( life_time <= 0.0f )
			{
				owner->kill();
			}
		}
	};
}