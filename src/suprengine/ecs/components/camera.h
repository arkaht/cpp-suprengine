#pragma once

#include <suprengine/ecs/component.h>

#include <suprengine/rect.h>
#include <suprengine/mtx4.h>

namespace suprengine
{
	constexpr float CAMERA_DEFAULT_ZNEAR { 0.01f }, 
					CAMERA_DEFAULT_ZFAR { 10000.0f };

	class Camera : public Component
	{
	protected:
		Mtx4 view_matrix;
		bool is_view_matrix_dirty = true;

		Vec3 offset {};
		Vec2 viewport_size {};

		bool try_register();
		void setup_vars();

	public:
		Mtx4 projection_matrix;

		Rect viewport { 0, 0, 0, 0 };
		float zoom { 1.0f };

		Rect clip { 0, 0, 0, 0 };
		bool clip_enabled { false };
		
		Camera( Entity* owner );
		Camera( Entity* owner, float fov, float znear = CAMERA_DEFAULT_ZNEAR, float zfar = CAMERA_DEFAULT_ZFAR );

		void setup_simple_projection();
		void setup_perspective( float fov, float znear, float zfar );
		void setup_perspective( float fov ) { setup_perspective( fov, CAMERA_DEFAULT_ZNEAR, CAMERA_DEFAULT_ZFAR ); }
		void look_at( const Vec3& target );

		void set_offset( const Vec3& offset );
		Vec3 get_offset() const { return offset; }
		void reset( float width, float height );

		const Mtx4& get_view_matrix();
	};
}