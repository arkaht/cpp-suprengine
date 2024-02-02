#pragma once

#include <components/stylized-model-renderer.h>

namespace puzzle
{
	using namespace suprengine;

	class Spaceship;

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
		float rotation_speed = 5.0f;
		float impact_distance = 4.0f;
		float damage_amount = 10.0f;
		//float knockback_force = 15.0f;

		Vec3 up_direction { Vec3::up };

	private:
		std::weak_ptr<Transform> _wk_target;
		Spaceship* _owner;

		std::shared_ptr<StylizedModelRenderer> _model_renderer;
	};
}