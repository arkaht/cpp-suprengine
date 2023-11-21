#pragma once
#include <components/stylized-model-renderer.hpp>

namespace puzzle
{
	using namespace suprengine;

	class PlayerSpaceship : public Entity
	{
	public:
		PlayerSpaceship();

		void update_this( float dt ) override;

	private:
		void _handle_movement( float dt );
		void _handle_shoot( float dt );

		Vec3 _previous_location;
		Vec2 _aim_velocity;
		Color _color = Color::green;

		const float SHOOT_TIME = 0.15f;
		float _shoot_time = 0.0f;

		std::shared_ptr<StylizedModelRenderer> model_renderer;

		Entity* camera_owner { nullptr };
		std::shared_ptr<Camera> camera;
	};
}