#pragma once

#include "rect.h"

namespace suprengine
{
	class Camera
	{
	public:
		Rect viewport { 0, 0, 0, 0 };
		float zoom { 1.0f };

		Rect clip { 0, 0, 0, 0 };
		bool clip_enabled { false };

		Camera() {}

		void translate( Vec2 pos ) { viewport.add_pos( pos ); }
		void reset( float width, float height )
		{
			viewport = { 0.0f, 0.0f, width, height };
			zoom = { 1.0f };

			clip = { 0.0f, 0.0f, 0.0f, 0.0f };
			clip_enabled = false;
		}

		Rect& get_viewport() { return viewport; }
	};
}