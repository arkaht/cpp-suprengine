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
	if ( target )
	{
		const Vec3 dir = 
			target->transform->location - _possessed_ship->transform->location;
		const Vec3 normalized_dir = dir.normalized();

		const Quaternion desired_rotation = Quaternion::look_at( 
			normalized_dir,
			Vec3::up
		);

		float forward_alignement = Vec3::dot( 
			normalized_dir, 
			_possessed_ship->transform->get_forward() 
		);

		//  shoot if aligned
		if ( _possessed_ship->get_shoot_time() <= 0.0f 
			&& forward_alignement >= 0.9f )
		{
			_possessed_ship->shoot();
		}

		_inputs.throttle_delta = forward_alignement;
		//printf( "%f\n", _inputs.throttle_delta );

		_inputs.desired_rotation = desired_rotation;

		_inputs.should_smooth_rotation = true;
		_inputs.smooth_rotation_speed = 2.0f;
	}
	else
	{
		_inputs.throttle_delta = 0.0f;
	}
}
