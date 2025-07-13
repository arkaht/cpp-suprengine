#pragma once

#include <suprengine/core/entity.h>
#include <suprengine/components/transform.h>
#include <suprengine/input/input-action.h>

namespace suprengine
{
	class Looker : public Component
	{
	public:
		float yaw { 0.0f }, pitch { 0.0f };

		InputAction<Vec2>* look_action = nullptr;

		float sensitivity = 0.25f;

		bool constrain_yaw = false;
		bool constrain_pitch = true;

		Vec2 yaw_range = Vec2 { -180.0f, 180.0f } * math::DEG2RAD;
		Vec2 pitch_range = Vec2 { -30.0f, 30.0f } * math::DEG2RAD;

	public:
		Looker(
			InputAction<Vec2>* look_action,
			float sensitivity = 0.25f,
			int priority_order = 0
		)
			: look_action( look_action ),
			  sensitivity( sensitivity ),
			  Component( priority_order )
		{}

		void setup() override
		{
			yaw = transform->rotation.get_radian_yaw();
			//pitch = transform->rotation.get_radian_pitch();
		}

		void update( float dt ) override
		{
			if ( look_action == nullptr ) return;

			const Vec2 input = look_action->get_value();
			if ( input == Vec2::zero ) return;

			const Vec2 rotation_rate = input * ( sensitivity * dt );

			// Compute yaw quaternion
			Quaternion new_rotation_with_yaw = transform->rotation + Quaternion(
				Vec3::up,
				rotation_rate.x
			);
			new_rotation_with_yaw.normalize();

			// Compute pitch quaternion
			const Vec3 right_vector = Vec3::transform( Vec3::right, new_rotation_with_yaw );
			Quaternion new_rotation_with_pitch = new_rotation_with_yaw + Quaternion(
				right_vector,
				rotation_rate.y
			);
			new_rotation_with_pitch.normalize();

			const float pitch = new_rotation_with_pitch.get_radian_pitch();
			if ( pitch > pitch_range.x && pitch < pitch_range.y )
			{
				transform->set_rotation( new_rotation_with_pitch );
			}
			else
			{
				// Correct overshooting pitch to clamp it
				const float clamped_pitch = math::clamp(
					pitch,
					pitch_range.x,
					pitch_range.y
				);
				const float pitch_diff = pitch - clamped_pitch;
				new_rotation_with_pitch = new_rotation_with_yaw + Quaternion(
					right_vector,
					rotation_rate.y - pitch_diff
				);
				new_rotation_with_pitch.normalize();

				transform->set_rotation( new_rotation_with_pitch );
			}
		}
	};
}