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
		Vec3 up_direction = Vec3::up;

	public:
		Camera() = default;
		explicit Camera(
			const CameraProjectionSettings& projection_settings
		);

		void setup() override;
		void unsetup() override;

		void setup_simple_projection();
		void setup_perspective( float fov, float znear, float zfar );
		void update_projection();

		void set_fov( float fov );

		const CameraProjectionSettings& get_projection_settings() const;

		/**
		 * Translate a 3D-world location into a 2D-viewport pixel position.
		 * Returns a Vec3 where Z > 0.0f helps to determine visibility.
		 */
		Vec3 world_to_viewport( const Vec3& location ) const;
		/**
		 * Translate a 2D-viewport pixel position into a 3D-world location.
		 * Returns a Ray starting at the near plane and ending at the far plane.
		 */
		Ray viewport_to_world( const Vec2& position ) const;

		void look_at( const Vec3& target );

		void set_offset( const Vec3& offset );
		Vec3 get_offset() const { return _offset; }

		void reset();

		void set_active();
		void set_inactive();
		bool is_active() const;

		/**
		 * Set the viewport specifying where the scene is going to be rendered inside the window.
		 * Expressed in normalized coordinates to automatically adapt with window resizing.
		 * @param viewport Rectangle in normalized coordinates.
		 */
		void set_viewport( const Rect& viewport );
		Rect get_viewport() const;

		/**
		 * Compute the screen viewport using the normalized screen-viewport.
		 * @return Screen viewport, in pixels.
		 */
		Rect get_screen_viewport() const;

		/**
		 * Override auto-computed view matrix for this frame.
		 * Needs to be updated each end of frame before the actual rendering occurs.
		 */
		void set_view_matrix( const Mtx4& matrix );
		const Mtx4& get_view_matrix() const;

		const Mtx4& get_projection_matrix() const;
		const Mtx4& get_viewport_matrix() const;

	private:
		void setup_vars();

		void _on_window_resized( const Vec2& new_size, const Vec2& old_size );

	private:
		Mtx4 _projection_matrix;
		Mtx4 _viewport_matrix;
		Mtx4 _view_matrix;

		bool _is_projection_matrix_dirty = true;
		bool _is_view_matrix_dirty = true;
		bool _is_active = false;

		CameraProjectionSettings _projection_settings {};

		/**
		 * Rectangle specifying where the scene is going to be rendered on screen.
		 * Expressed in normalized coordinates to automatically adapt with window resizing.
		 */
		Rect _viewport { 0, 0, 1.0f, 1.0f };

		Vec3 _offset {};
	};
}