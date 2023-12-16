#pragma once
#include <suprengine/component.h>

namespace puzzle
{
	using namespace suprengine;

	class Spaceship;

	class SpaceshipController : public Component
	{
	public:
		SpaceshipController( Spaceship* owner );

		void setup() override;
		void unsetup() override;

	protected:
		Spaceship* _owned_ship;
	};
}