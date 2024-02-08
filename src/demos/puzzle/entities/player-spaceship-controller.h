#pragma once
#include <entities/spaceship.h>

namespace puzzle
{
	using namespace suprengine;

	class PlayerSpaceshipController : public SpaceshipController
	{
	public:
		PlayerSpaceshipController();
		~PlayerSpaceshipController();

		void update_this( float dt ) override;

		void on_possess() override;
		void on_unpossess() override;

		void update_inputs( float dt ) override;

	public:
		//  TODO: remove and replace by auto-target system
		std::weak_ptr<HealthComponent> wk_missile_target;

	private:
		//  Aim sensitivity for each axis
		const Vec3 AIM_SENSITIVITY { 
			0.09f,		//  mouse-x: roll
			0.09f,		//  mouse-y: pitch
			0.10f		//  Q-D: yaw
		};
		//  Aim velocity loss per second
		const float AIM_VELOCITY_DECREASE = 5.0f;
		//  Max aim velocity
		const float MAX_AIM_VELOCITY = 10.0f;

		//  Camera FOV depending on throttle
		const Vec2 CAMERA_FOV {
			77.0f,		//  at min throttle 
			99.0f		//  at max throttle
		};
		//  Camera backward distance depending on throttle
		const Vec2 CAMERA_BACKWARD { 
			6.0f,		//  at min throttle 
			2.0f		//  at max throttle
		};
		//  Camera distance when looking backward depending on throttle
		const Vec2 CAMERA_LOOK_BACKWARD_DISTANCE {
			12.0f,		//  at min throttle
			25.0f		//  at max throttle
		};
		//  Camera smooth movement speed depending on throttle
		const Vec2 CAMERA_MOVE_SPEED { 
			7.0f,		//  at min throttle 
			12.0f		//  at max throttle
		};
		//  Camera up distance depending on throttle
		const Vec2 CAMERA_UP_RANGE { 
			2.0f,		//  at min throttle
			4.0f		//  at max throttle
		};
		//  Camera smooth rotation speed
		const float CAMERA_ROTATION_SPEED = 10.0f;

	private:
		void _update_shoot( float dt );
		void _update_camera( float dt );

	private:
		Vec3 _aim_velocity = Vec3::zero;

		std::shared_ptr<Camera> camera;
	};
}