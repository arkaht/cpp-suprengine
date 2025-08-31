#pragma once

#include <suprengine/components/input-component.h>
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
			const SharedPtr<InputComponent>& input_component,
			InputAction<Vec2>* look_action,
			const float sensitivity = 0.25f,
			const int priority_order = 0
		)
			: _input_component( input_component ),
			  look_action( look_action ),
			  sensitivity( sensitivity ),
			  Component( priority_order )
		{
			ASSERT( _input_component != nullptr );
		}

		void setup() override
		{
			yaw = transform->rotation.get_radian_yaw();
			//pitch = transform->rotation.get_radian_pitch();
		}

		void update( const float dt ) override
		{
			const Vec2 input = _input_component->read_value( look_action );
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

	private:
		SharedPtr<InputComponent> _input_component = nullptr;
	};
}