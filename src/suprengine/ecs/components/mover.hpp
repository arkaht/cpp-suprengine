#pragma once
#include <suprengine/ecs/component.h>

#include <suprengine/ecs/entity.h>
#include <suprengine/ecs/components/transform.hpp>

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
			Game* game = owner->get_game();
			Vec3 dir {};

			//  forward/backward
			if ( game->is_key_down( SDL_SCANCODE_W ) || game->is_key_down( SDL_SCANCODE_UP ) )
				dir += transform->get_forward();	
			if ( game->is_key_down( SDL_SCANCODE_S ) || game->is_key_down( SDL_SCANCODE_DOWN ) )
				dir -= transform->get_forward();
			//  right/left
			if ( game->is_key_down( SDL_SCANCODE_D ) || game->is_key_down( SDL_SCANCODE_RIGHT ) )
				dir += transform->get_right();
			if ( game->is_key_down( SDL_SCANCODE_A ) || game->is_key_down( SDL_SCANCODE_LEFT ) )
				dir -= transform->get_right();
			//  up/down
			if ( game->is_key_down( SDL_SCANCODE_E ) )
				dir += transform->get_up();
			if ( game->is_key_down( SDL_SCANCODE_Q ) )
				dir -= transform->get_up();

			if ( dir == Vec3::zero )
				return;

			float speed = ( game->is_key_down( SDL_SCANCODE_LSHIFT ) ? sprint_speed : move_speed ) * dt;
			transform->set_location( transform->location - dir.normalized() * speed );
		}
	};
}