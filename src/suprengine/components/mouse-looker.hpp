#pragma once

#include <suprengine/entity.h>
#include <suprengine/components/transform.h>

namespace suprengine
{
	class MouseLooker : public Component
	{
	public:
		float yaw { 0.0f }, pitch { 0.0f };

		float sensitivity;

		bool constrain_yaw = false;
		bool constrain_pitch = true;

		Vec2 yaw_range = Vec2 { -180.0f, 180.0f } * math::DEG2RAD;
		Vec2 pitch_range = Vec2 { -60.0f, 60.0f } * math::DEG2RAD;

		MouseLooker( Entity* owner, float sensitivity = 0.25f, int priority_order = 0 )
			: sensitivity( sensitivity ), Component( owner, priority_order ) 
		{
			yaw = owner->transform->rotation.get_y_angle();
			//pitch = owner->transform->rotation.get_x_angle();
		}

		void update( float dt ) override
		{
			//  get mouse delta
			InputManager* _inputs = owner->get_game()->get_inputs();
			Vec2 delta = _inputs->mouse_delta;
			if ( delta == Vec2::zero ) return;

			//  compute yaw
			yaw = math::normalize_angle( yaw + delta.x * sensitivity * dt );
			if ( constrain_yaw )
				yaw = math::clamp( yaw, yaw_range.x, yaw_range.y );

			//  compute pitch
			pitch = math::normalize_angle( pitch - delta.y * sensitivity * dt );
			if ( constrain_pitch )
				pitch = math::clamp( pitch, pitch_range.x, pitch_range.y );

			//  compute rotation
			Quaternion yaw_step( Vec3::up, yaw ),
					   pitch_step( transform->get_right(), pitch );

			//  apply new rotation
			transform->set_rotation( yaw_step + pitch_step );
		}
	};
}