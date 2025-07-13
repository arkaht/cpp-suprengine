#pragma once

#include <suprengine/core/engine.h>

#include <suprengine/components/transform.h>

namespace suprengine
{
	class Mover : public Component
	{
	public:
		explicit Mover(
			InputAction<Vec2>* move_action,
			InputAction<bool>* sprint_action = nullptr
		)
			: move_action( move_action ), sprint_action( sprint_action ) {}

		void update( const float dt ) override
		{
			if ( move_action == nullptr ) return;

			const Vec2 move_input = move_action->get_value();
			if ( move_input == Vec2::zero ) return;

			const Vec3 dir = transform->get_forward() * move_input.y
			               + transform->get_right()   * move_input.x;

			//  get movement
			const float speed = sprint_action != nullptr && sprint_action->get_value()
				? sprint_speed
				: move_speed;
			const Vec3 move_dir = dir * speed * dt;
			Vec3 pos = transform->location + move_dir;

			//  check collisions
			if ( should_collide )
			{
				Physics* physics = Engine::instance().get_physics();
				
				//  setup raycast
				Ray ray( transform->location, move_dir );
				RayParams params {};
				params.can_hit_from_origin = true;

				//  correct position from raycast
				RayHit hit {};
				if ( physics->raycast( ray, &hit, params ) )
				{
					pos = hit.point + hit.normal * 0.01f;
				}
			}

			//  apply movement
			transform->set_location( pos );
		}

	public:
		float move_speed = 10.0f;
		float sprint_speed = 25.0f;
		bool should_collide = true;

		InputAction<Vec2>* move_action = nullptr;
		InputAction<bool>* sprint_action = nullptr;
	};
}