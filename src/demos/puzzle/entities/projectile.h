#pragma once
#include <components/stylized-model-renderer.hpp>

namespace puzzle
{
	using namespace suprengine;

	class PlayerSpaceship;

	class Projectile : public Entity
	{
	public:
		Projectile( PlayerSpaceship* owner, Color color );

		void update_this( float dt ) override;

		void on_hit( const RayHit& result );

	public:
		float move_speed = 750.0f;
		float life_time = 2.5f;

		float damage_amount = 5.0f; 
		float knockback_force = 80.0f;

	private:
		Color _color;

		PlayerSpaceship* _owner;

		std::shared_ptr<StylizedModelRenderer> model_renderer;
	};
}

