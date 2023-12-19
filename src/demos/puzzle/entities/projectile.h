#pragma once

#include <components/stylized-model-renderer.h>

namespace puzzle
{
	using namespace suprengine;

	class Spaceship;

	class Projectile : public Entity
	{
	public:
		Projectile( Spaceship* owner, Color color );

		void update_this( float dt ) override;

	public:
		float move_speed = 750.0f;
		float life_time = 2.5f;

		float damage_amount = 5.0f; 
		float knockback_force = 80.0f;

	private:
		bool _check_collisions( float movement_speed );
		void _on_hit( const RayHit& result );

	private:
		Color _color;

		Spaceship* _owner;

		std::shared_ptr<StylizedModelRenderer> model_renderer;
	};
}

