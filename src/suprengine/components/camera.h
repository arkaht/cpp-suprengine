#pragma once

#include <suprengine/core/component.h>

#include <suprengine/math/rect.h>
#include <suprengine/math/mtx4.h>

#include <suprengine/utils/ray.h>

namespace suprengine
{
	constexpr float CAMERA_DEFAULT_ZNEAR { 0.01f }, 
					CAMERA_DEFAULT_ZFAR { 10000.0f };

	struct CameraProjectionSettings
	{
		float fov = 77.7f;
		float znear = CAMERA_DEFAULT_ZNEAR;
		float zfar = CAMERA_DEFAULT_ZFAR;
	};

	class Camera : public Component
	{
	public:
		CameraProjectionSettings projection_settings;

		Rect viewport { 0, 0, 0, 0 };
		float zoom { 1.0f };

		Rect clip { 0, 0, 0, 0 };
		bool clip_enabled { false };

		Vec3 up_direction = Vec3::up;
		
	public:
		Camera();
		Camera(
			const CameraProjectionSettings& projection_settings
		);

		void setup() override;

		void setup_simple_projection();
		void setup_perspective( float fov, float znear, float zfar );
		void setup_perspective( float fov ) { setup_perspective( fov, CAMERA_DEFAULT_ZNEAR, CAMERA_DEFAULT_ZFAR ); }
		void update_projection_from_settings();

		void set_fov( float fov );

		/*
		 * Translate a 3D-world location into a 2D-viewport position.
		 * Returns a Vec3 where Z > 0.0f helps to determine visibility.
		 */
		Vec3 world_to_viewport( const Vec3& location ) const;
		/*
		 * Translate a 2D-viewport position into a 3D-world location.
		 * Returns a Ray starting at the near plane and ending at the far plane.
		 */
		Ray viewport_to_world( const Vec2& position ) const;

		void look_at( const Vec3& target );

		void set_offset( const Vec3& offset );
		Vec3 get_offset() const { return _offset; }

		void reset( float width, float height );

		void activate();
		bool is_active();

		/*
		 * Override auto-computed view matrix. 
		 * Needs to be updated each end of frame before the actual rendering occurs.
		 */
		void set_view_matrix( const Mtx4& matrix );

		const Mtx4& get_view_matrix();
		const Mtx4& get_projection_matrix();

	private:
		void setup_vars();

		void _on_window_resized( const Vec2& new_size, const Vec2& old_size );

	private:
		Mtx4 _projection_matrix;
		Mtx4 _view_matrix;
		bool _is_view_matrix_dirty = true;

		Vec3 _offset {};
		Vec2 _viewport_size {};
	};
}