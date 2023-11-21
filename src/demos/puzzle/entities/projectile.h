#pragma once
#include <components/stylized-model-renderer.hpp>

namespace puzzle
{
	using namespace suprengine;

	class Projectile : public Entity
	{
	public:
		float move_speed = 125.0f;
		float life_time = 2.5f;

		Projectile( Color color );

		void update_this( float dt ) override;

	private:
		Color _color;

		std::shared_ptr<StylizedModelRenderer> model_renderer;
	};
}

