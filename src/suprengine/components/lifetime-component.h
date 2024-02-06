#pragma once

#include <suprengine/component.h>

namespace suprengine
{
	class LifetimeComponent : public Component
	{
	public:
		LifetimeComponent( Entity* owner, float life_time = 3.0f );

		void update( float dt ) override;

	public:
		/*
		 * Called when the lifetime has expired.
		 */
		Event<> on_time_out;

	public:
		float life_time = 0.0f;
	};
}