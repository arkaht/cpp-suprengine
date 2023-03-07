#pragma once

#include <suprengine/ecs/entity.h>
#include <suprengine/ecs/components/transform.h>

namespace suprengine
{
	class MouseLooker : Component
	{
	public:
		float sensitivity;

		MouseLooker( Entity* owner, float sensitivity = 0.25f, int priority_order = 0 ) 
			: sensitivity( sensitivity ), Component( owner, priority_order ) {}

		void update( float dt ) override
		{
			InputManager* inputs = owner->get_game()->get_inputs();
			Vec2 delta = inputs->get_mouse_delta();

			Quaternion new_rotation = transform->rotation;
			new_rotation = Quaternion::concatenate( new_rotation, Quaternion( Vec3::up, delta.x * sensitivity * dt ) );
			new_rotation = Quaternion::concatenate( new_rotation, Quaternion( Vec3::right, -delta.y * sensitivity * dt ) );
			transform->set_rotation( new_rotation );
		}
	};
}