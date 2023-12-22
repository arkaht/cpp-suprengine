#include "ai-spaceship-controller.h"

using namespace puzzle;

AISpaceshipController::AISpaceshipController()
{
}

AISpaceshipController::~AISpaceshipController()
{
}

void AISpaceshipController::update_this( float dt )
{
}

void AISpaceshipController::on_possess()
{
}

void AISpaceshipController::on_unpossess()
{
}

void AISpaceshipController::update_inputs( float dt )
{
	_inputs.throttle_delta = 0.0f;

	if ( target )
	{
		_inputs.desired_rotation = Quaternion::look_at( 
			target->transform->location,
			_possessed_ship->transform->location,
			Vec3::up
		);
	}
}
