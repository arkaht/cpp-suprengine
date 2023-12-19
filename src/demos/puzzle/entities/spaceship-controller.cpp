#include "spaceship-controller.h"
#include "entities/spaceship.h"

using namespace puzzle;

SpaceshipController::~SpaceshipController()
{
	unpossess();
}

void SpaceshipController::possess( Spaceship* ship )
{
	auto old = _possessed_ship;

	_suppress_event = true;
	unpossess();
	_suppress_event = false;

	//auto ptr = get_shared_from_this<SpaceshipController>();
	_possessed_ship = ship;
	_possessed_ship->controller = this;
	on_possess();

	//  invoke event
	event_on_possess_changed.invoke( old, _possessed_ship );
}

void SpaceshipController::unpossess()
{
	//auto ptr = get_shared_from_this<SpaceshipController>();
	if ( !_possessed_ship ) return;

	//  reset controller of owned ship
	if ( _possessed_ship->controller == this )
	{
		_possessed_ship->controller = nullptr;
	}

	//  reset pointer
	auto old = _possessed_ship;
	_possessed_ship = nullptr;
	on_unpossess();

	//  invoke event
	if ( !_suppress_event )
	{
		event_on_possess_changed.invoke( old, nullptr );
	}
}
