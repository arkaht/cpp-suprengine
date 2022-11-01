#pragma once

#include "rect.h"

namespace suprengine
{
	class Camera
	{
	public:
		Rect viewport { 0, 0, 0, 0 };
		float zoom { 1.0f };

		Camera() {}

		void translate( Vec2 pos ) { viewport.add_pos( pos ); }

		Rect& get_viewport() { return viewport; }
	};
}