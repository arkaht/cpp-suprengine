#pragma once
#include <components/stylized-model-renderer.hpp>

namespace puzzle
{
	using namespace suprengine;

	class Projectile : public Entity
	{
	public:
		float move_speed = 500.0f;
		float life_time = 2.5f;

		float damage_amount = 5.0f; 
		float knockback_force = 80.0f;

		Projectile( Color color );

		void update_this( float dt ) override;

		void on_hit( const RayHit& result );

	private:
		Color _color;

		std::shared_ptr<StylizedModelRenderer> model_renderer;
	};
}

