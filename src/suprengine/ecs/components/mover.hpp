#pragma once
#include <suprengine/ecs/component.h>

#include <suprengine/ecs/entity.h>
#include <suprengine/ecs/components/transform.h>

namespace suprengine
{
	class Mover : public Component
	{
	public:
		float move_speed = 10.0f; 
		float sprint_speed = 25.0f;
		bool should_collide = true;

		Mover( Entity* owner ) : Component( owner ) {}

		void update( float dt ) override
		{
			InputManager* inputs = owner->get_game()->get_inputs();
			Vec3 dir {};

			//  forward/backward
			if ( inputs->is_key_down( SDL_SCANCODE_W ) || inputs->is_key_down( SDL_SCANCODE_UP ) )
				dir += transform->get_forward();
			if ( inputs->is_key_down( SDL_SCANCODE_S ) || inputs->is_key_down( SDL_SCANCODE_DOWN ) )
				dir -= transform->get_forward();

			//  right/left
			if ( inputs->is_key_down( SDL_SCANCODE_D ) || inputs->is_key_down( SDL_SCANCODE_RIGHT ) )
				dir += transform->get_right();
			if ( inputs->is_key_down( SDL_SCANCODE_A ) || inputs->is_key_down( SDL_SCANCODE_LEFT ) )
				dir -= transform->get_right();

			//  up/down
			if ( inputs->is_key_down( SDL_SCANCODE_E ) )
				dir += Vec3::up;
			if ( inputs->is_key_down( SDL_SCANCODE_Q ) )
				dir -= Vec3::up;

			if ( dir == Vec3::zero ) return;

			//  get movement
			float speed = ( inputs->is_key_down( SDL_SCANCODE_LSHIFT ) ? sprint_speed : move_speed ) * dt;
			Vec3 move_dir = dir.normalized() * speed;
			Vec3 pos = transform->location + move_dir;

			//  check collisions
			if ( should_collide )
			{
				Physics* physics = owner->get_game()->get_physics();
				
				//  setup raycast
				Ray ray( transform->location, move_dir );
				RayParams params;
				params.can_hit_from_origin = true;

				//  correct position from raycast
				RayHit hit;
				if ( physics->raycast( ray, &hit, params ) )
				{
					pos = hit.point + hit.normal * 0.01f;
				}
			}

			//  apply movement
			transform->set_location( pos );
		}
	};
}