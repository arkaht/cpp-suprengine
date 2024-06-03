#pragma once
#include <entities/spaceship.h>

namespace puzzle
{
	using namespace suprengine;

	class AISpaceshipController : public SpaceshipController
	{
	public:
		AISpaceshipController();
		~AISpaceshipController();

		void update_this( float dt ) override;

		void on_possess() override;
		void on_unpossess() override;

		void update_inputs( float dt ) override;

	public:
		Spaceship* target { nullptr };
	};
}