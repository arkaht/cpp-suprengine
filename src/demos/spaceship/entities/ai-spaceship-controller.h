#pragma once
#include <entities/spaceship.h>

namespace spaceship
{
	using namespace suprengine;

	class AISpaceshipController : public SpaceshipController
	{
	public:
		AISpaceshipController();

		void update_inputs( float dt ) override;

	public:
		weak_ptr<Spaceship> wk_target;
	};
}