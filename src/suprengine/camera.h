#pragma once

#include <suprengine/rect.h>
#include <suprengine/mtx4.h>

namespace suprengine
{
	class Camera
	{
	private:
		Mtx4 matrix;
		bool is_matrix_dirty = true;

	public:
		Rect viewport { 0, 0, 0, 0 };
		float zoom { 1.0f };

		Rect clip { 0, 0, 0, 0 };
		bool clip_enabled { false };
		
		Camera() {}

		void translate( Vec2 pos ) 
		{ 
			viewport.add_pos( pos );
			is_matrix_dirty = true;
		}
		void reset( float width, float height )
		{
			viewport = { 0.0f, 0.0f, width, height };
			zoom = 1.0f;

			clip = { 0.0f, 0.0f, 0.0f, 0.0f };
			clip_enabled = false;

			is_matrix_dirty = true;
		}

		const Mtx4& get_matrix();

		Rect& get_viewport() { return viewport; }
	};
}