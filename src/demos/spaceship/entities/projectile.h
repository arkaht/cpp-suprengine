#pragma once

#include <components/stylized-model-renderer.h>

#include <suprengine/components/lifetime-component.h>

namespace spaceship
{
	using namespace suprengine;

	class Spaceship;

	class Projectile : public Entity
	{
	public:
		Projectile( shared_ptr<Spaceship> owner, Color color );

		void setup() override;
		void update_this( float dt ) override;

	public:
		float move_speed = 750.0f;

		float damage_amount = 5.0f; 
		float knockback_force = 80.0f;

	private:
		bool _check_collisions( float movement_speed );
		void _on_hit( const RayHit& result );

	private:
		const float LIFETIME = 3.0f;
	
	private:
		Color _color;

		weak_ptr<Spaceship> _wk_owner;

		shared_ptr<StylizedModelRenderer> _model_renderer;
		shared_ptr<LifetimeComponent> _lifetime_component;
	};
}

