#pragma once
#include <suprengine/ecs/component.h>

#include <suprengine/ecs/entity.h>
#include <suprengine/ecs/components/transform.h>

namespace suprengine
{
	class Mover : public Component
	{
	public:
		float move_speed = 10.0f, 
			  sprint_speed = 25.0f;

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

			//  apply movement
			float speed = ( inputs->is_key_down( SDL_SCANCODE_LSHIFT ) ? sprint_speed : move_speed ) * dt;
			Vec3 move_dir = dir.normalized() * speed;
			transform->set_location( transform->location + move_dir );
		}
	};
}