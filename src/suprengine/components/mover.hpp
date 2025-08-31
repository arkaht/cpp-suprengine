#pragma once

#include <suprengine/core/engine.h>

#include <suprengine/components/transform.h>
#include <suprengine/components/input-component.h>

namespace suprengine
{
	class Mover : public Component
	{
	public:
		explicit Mover(
			const SharedPtr<InputComponent>& input_component,
			InputAction<Vec2>* move_action,
			InputAction<bool>* sprint_action = nullptr,
			InputAction<float>* vertical_action = nullptr
		)
			: move_action( move_action ),
			  sprint_action( sprint_action ),
			  vertical_action( vertical_action ),
			  _input_component( input_component ) {}

		void update( const float dt ) override
		{
			Vec3 dir = Vec3::zero;

			const Vec2 move_input = _input_component->read_value( move_action );
			if ( move_input != Vec2::zero )
			{
				dir = transform->get_forward() * move_input.y
					+ transform->get_right() * move_input.x;
			}

			if ( vertical_action != nullptr )
			{
				dir += transform->get_up() * _input_component->read_value( vertical_action );
			}

			if ( dir == Vec3::zero ) return;

			//  get movement
			const float speed = _input_component->read_value( sprint_action )
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
		InputAction<float>* vertical_action = nullptr;

	private:
		SharedPtr<InputComponent> _input_component = nullptr;
	};
}
