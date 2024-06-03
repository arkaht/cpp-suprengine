#pragma once

#include <suprengine/component.h>

namespace eks
{
	using namespace suprengine;

	class CameraController : public Component
	{
	public:
		CameraController( 
			Entity* owner, 
			float move_speed = 20.0f,
			const Vec3& offset = Vec3::zero
		);

		void update( float dt ) override;
	
	public:
		float move_speed;
		Vec3 offset;
		weak_ref<Transform> focus_target;
	};
}