#pragma once
#include <components/stylized-model-renderer.hpp>

namespace puzzle
{
	using namespace suprengine;

	class Spaceship : public Entity
	{
	public:
		Spaceship();
		virtual ~Spaceship();

		void update_this( float dt ) override;

	public:
		std::weak_ptr<SpaceshipController> controller;

	private:
		const float MAX_THROTTLE_SPEED = 200.0f;
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
		Color _color = Color::green;

		float _shoot_time = 0.0f;

		std::shared_ptr<StylizedModelRenderer> model_renderer;
		std::shared_ptr<StylizedModelRenderer> trail_renderer;
	};
}