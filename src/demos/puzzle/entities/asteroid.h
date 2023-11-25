#pragma once

#include <suprengine/components/colliders/sphere-collider.h>
#include <components/stylized-model-renderer.hpp>

namespace puzzle
{
	using namespace suprengine;

	class Asteroid : public Entity
	{
	public:
		Vec3 linear_direction = Vec3::forward * 5.0f;
		int split_times = 2;

		Asteroid();
		~Asteroid();

		void update_this( float dt ) override;

		void update_collision_to_transform();

		void damage( float amount, const Vec3& knockback );
		void split();

	private:
		float _health;

		std::shared_ptr<StylizedModelRenderer> _model_renderer;
		std::shared_ptr<SphereCollider> _collider;
	};
}