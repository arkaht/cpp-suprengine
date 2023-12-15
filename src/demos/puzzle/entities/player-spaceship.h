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
		const Vec3 AIM_SENSITIVITY { 0.3f, 0.1f, 0.075f };
		const float MAX_AIM_VELOCITY = 2.0f;
		const float MAX_THROTTLE_SPEED = 200.0f;

		const Vec2 CAMERA_BACKWARD_RANGE { 6.0f, -3.0f };
		const Vec2 CAMERA_SPEED_RANGE { 7.0f, 12.0f };
		const Vec2 CAMERA_UP_RANGE { 2.0f, 4.0f };

		const float SHOOT_TIME = 0.15f;

	private:
		void _update_movement( float dt );
		void _update_trail( float dt, float time );
		void _update_camera( float dt );
		void _update_shoot( float dt );

	private:
		Vec3 _previous_location;
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