#pragma once

#include <components/stylized-model-renderer.h>

#include <suprengine/components/lifetime-component.h>

namespace puzzle
{
	using namespace suprengine;

	class Spaceship;
	class HealthComponent;

	class GuidedMissile : public Entity
	{
	public:
		GuidedMissile( 
			Spaceship* owner,
			std::weak_ptr<Transform> wk_target, 
			Color color 
		);

		void update_this( float dt ) override;

	public:
		float move_speed = 150.0f;
		float move_acceleration = 16.0f;

		float rotation_speed = 30.0f;
		float rotation_acceleration = 2.0f;

		float impact_distance = 4.0f;
		float damage_amount = 15.0f;
		float knockback_force = 45.0f;

		Vec3 up_direction { Vec3::up };

	private:
		void _update_target( float dt );
		void _check_impact();

		void _damage( std::shared_ptr<HealthComponent> target );

	private:
		const float LIFETIME = 6.0f;

	private:
		float _current_move_speed { 0.0f };
		float _current_rotation_speed { 0.0f };

		std::weak_ptr<Transform> _wk_target;
		Spaceship* _owner;

		std::shared_ptr<StylizedModelRenderer> _model_renderer;
		std::shared_ptr<LifetimeComponent> _lifetime_component;
	};
}