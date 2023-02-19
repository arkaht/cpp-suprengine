#pragma once

#include <suprengine/rect.h>
#include <suprengine/mtx4.h>

namespace suprengine
{
	class Camera
	{
	protected:
		Mtx4 view_matrix;
		bool is_view_matrix_dirty = true;

		Vec3 location { 0.0f, 0.0f, 0.0f };

	public:
		const float DEFAULT_ZNEAR { 0.01f }, DEFAULT_ZFAR { 10000.0f };

		Mtx4 projection_matrix;

		Rect viewport { 0, 0, 0, 0 };
		float zoom { 1.0f };

		Rect clip { 0, 0, 0, 0 };
		bool clip_enabled { false };
		
		Camera() {}

		void setup_simple_projection();
		void setup_perspective( float fov, float znear, float zfar );
		void setup_perspective( float fov ) 
		{ 
			setup_perspective( fov, DEFAULT_ZNEAR, DEFAULT_ZFAR ); 
		}
		void look_at( const Vec3& target );

		void translate( Vec2 pos ) 
		{ 
			viewport.add_pos( pos );
			is_view_matrix_dirty = true;
		}
		void reset( float width, float height )
		{
			viewport = { 0.0f, 0.0f, width, height };
			zoom = 1.0f;

			clip = { 0.0f, 0.0f, 0.0f, 0.0f };
			clip_enabled = false;

			setup_simple_projection();
			is_view_matrix_dirty = true;
		}

		const Mtx4& get_view_matrix();
	};
}