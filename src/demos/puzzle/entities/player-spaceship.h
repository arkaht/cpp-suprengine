#pragma once
#include <components/stylized-model-renderer.hpp>

namespace puzzle
{
	using namespace suprengine;

	class PlayerSpaceship : public Entity
	{
	public:
		PlayerSpaceship();
		~PlayerSpaceship();

		void update_this( float dt ) override;

	private:
		//  Aim sensitivity for each axis
		const Vec3 AIM_SENSITIVITY { 
			0.10f,		//  mouse-x: roll
			0.10f,		//  mouse-y: pitch
			0.10f		//  Q-D: yaw
		};
		//  Aim velocity loss per second
		const float AIM_VELOCITY_DECREASE = 5.0f;
		//  Max aim velocity
		const float MAX_AIM_VELOCITY = 10.0f;

		//  Throttle gain per second
		const float THROTTLE_GAIN_SPEED = 0.65f;
		//  Max throttle offset by keeping forward pressed
		const float MAX_THROTTLE_FORWARD_OFFSET = 0.25f;
		/*
		 * Speed at maximum throttle (i.e. 1.0), 
		 * proportionally scaled before and beyond 
		 */
		const float MAX_THROTTLE_SPEED = 200.0f;

		//  Camera FOV depending on throttle
		const Vec2 CAMERA_FOV {
			77.0f,		//  at min throttle 
			99.0f		//  at max throttle
		};
		//  Camera backward distance depending on throttle
		const Vec2 CAMERA_BACKWARD { 
			6.0f,		//  at min throttle 
			-3.0f		//  at max throttle
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

		//  Shoot time interval
		const float SHOOT_TIME = 0.15f;

	private:
		void _update_movement( float dt );
		void _update_trail( float dt, float time );
		void _update_camera( float dt );
		void _update_shoot( float dt );

	private:
		float _throttle = 0.0f;
		float _trail_intensity = 0.0f;
		Vec3 _aim_velocity;
		Color _color = Color::green;

		float _shoot_time = 0.0f;

		std::shared_ptr<StylizedModelRenderer> model_renderer;
		std::shared_ptr<StylizedModelRenderer> trail_renderer;

		std::shared_ptr<Camera> camera;
		std::shared_ptr<Camera> second_camera;
	};
}