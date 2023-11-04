#pragma once
#include <suprengine/component.h>

#include <suprengine/entity.h>
#include <suprengine/components/transform.h>

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
			InputManager* _inputs = owner->get_game()->get_inputs();
			Vec3 dir {};

			//  forward/backward
			if ( _inputs->is_key_down( SDL_SCANCODE_W ) || _inputs->is_key_down( SDL_SCANCODE_UP ) )
				dir += transform->get_forward();
			if ( _inputs->is_key_down( SDL_SCANCODE_S ) || _inputs->is_key_down( SDL_SCANCODE_DOWN ) )
				dir -= transform->get_forward();

			//  right/left
			if ( _inputs->is_key_down( SDL_SCANCODE_D ) || _inputs->is_key_down( SDL_SCANCODE_RIGHT ) )
				dir += transform->get_right();
			if ( _inputs->is_key_down( SDL_SCANCODE_A ) || _inputs->is_key_down( SDL_SCANCODE_LEFT ) )
				dir -= transform->get_right();

			//  up/down
			if ( _inputs->is_key_down( SDL_SCANCODE_E ) )
				dir += Vec3::up;
			if ( _inputs->is_key_down( SDL_SCANCODE_Q ) )
				dir -= Vec3::up;

			if ( dir == Vec3::zero ) return;

			//  get movement
			float speed = ( _inputs->is_key_down( SDL_SCANCODE_LSHIFT ) ? sprint_speed : move_speed ) * dt;
			Vec3 move_dir = dir.normalized() * speed;
			Vec3 pos = transform->location + move_dir;

			//  check collisions
			if ( should_collide )
			{
				Physics* _physics = owner->get_game()->get_physics();
				
				//  setup raycast
				Ray ray( transform->location, move_dir );
				RayParams params;
				params.can_hit_from_origin = true;

				//  correct position from raycast
				RayHit hit;
				if ( _physics->raycast( ray, &hit, params ) )
				{
					pos = hit.point + hit.normal * 0.01f;
				}
			}

			//  apply movement
			transform->set_location( pos );
		}
	};
}