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
		const Vec3 AIM_SENSITIVITY { 
			0.15f,		//  mouse-x: roll
			0.07f,		//  mouse-y: pitch
			0.10f		//  Q-D: yaw
		};
		const float AIM_VELOCITY_DECREASE = 6.0f;
		const float MAX_AIM_VELOCITY = 2.0f;
		const float MAX_THROTTLE_SPEED = 200.0f;

		const Vec2 CAMERA_BACKWARD { 
			6.0f,		//  at min throttle 
			1.0f		//  at max throttle
		};
		const Vec2 CAMERA_MOVE_SPEED { 
			7.0f,		//  at min throttle 
			12.0f		//  at max throttle
		};
		const Vec2 CAMERA_UP_RANGE { 
			2.0f,		//  at min throttle
			4.0f		//  at max throttle
		};
		const float CAMERA_ROTATION_SPEED = 10.0f;

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