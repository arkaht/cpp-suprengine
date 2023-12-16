#include "spaceship-controller.h"
#include "entities/spaceship.h"

using namespace puzzle;

SpaceshipController::SpaceshipController( Spaceship* owner )
	: Component( owner )
{
	_owned_ship = owner;
}

void SpaceshipController::setup()
{
	auto ptr = get_shared_from_this<SpaceshipController>();
	_owned_ship->controller = ptr;
}

void SpaceshipController::unsetup()
{
	auto ptr = get_shared_from_this<SpaceshipController>();
	if ( _owned_ship->controller.lock() == ptr )
	{
		_owned_ship->controller.reset();
	}
}
