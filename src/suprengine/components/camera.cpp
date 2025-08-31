#include "camera.h"

#include <suprengine/core/engine.h>

#include <suprengine/components/transform.h>

using namespace suprengine;

Camera::Camera( const CameraProjectionSettings& projection_settings ) :
	projection_settings( projection_settings )
{
}

void Camera::setup()
{
	setup_vars();
	update_projection_from_settings();

	auto& engine = Engine::instance();
	engine.add_camera( this );
}

void Camera::unsetup()
{
	auto& engine = Engine::instance();
	engine.remove_camera( this );
}

void Camera::setup_simple_projection()
{
	const Rect screen_viewport = get_screen_viewport();
	_projection_matrix = Mtx4::create_simple_view_projection(
		screen_viewport.w,
		screen_viewport.h
	);
}

void Camera::setup_perspective( const float fov, const float znear, const float zfar )
{
	const Rect screen_viewport = get_screen_viewport();
	_projection_matrix = Mtx4::create_perspective_fov(
		fov * math::DEG2RAD,
		screen_viewport.w, screen_viewport.h,
		znear, zfar
	);
}

void Camera::update_projection_from_settings()
{
	setup_perspective(
		projection_settings.fov,
		projection_settings.znear,
		projection_settings.zfar
	);
}

void Camera::set_fov( const float fov )
{
	if ( fov == projection_settings.fov ) return;

	projection_settings.fov = fov;
	update_projection_from_settings();
}

Vec3 Camera::world_to_viewport( const Vec3& location ) const
{
	Vec3 pos = location;

	//	Pass location to view & projection matrices
	pos = Vec3::transform( pos, _view_matrix );
	pos = Vec3::transform( pos, _projection_matrix );

	//	Normalize by Z-axis
	pos.x /= pos.z;
	pos.y /= pos.z;

	//	Remap [-1.0f; 1.0f] to [0.0f; 1.0f]
	pos.x = ( pos.x + 1.0f ) * 0.5f;
	pos.y = ( pos.y + 1.0f ) * 0.5f;

	//	Convert to viewport
	const Rect screen_viewport = get_screen_viewport();
	pos.x *= screen_viewport.w;
	pos.y *= screen_viewport.h;
	pos.x += screen_viewport.x;
	pos.y += screen_viewport.y;

	return pos;
}

Ray Camera::viewport_to_world( const Vec2& position ) const
{
	//	Normalize the viewport-space position of range: 0.0f:1.0f
	const Rect screen_viewport = get_screen_viewport();
	const Vec2 normalized_screen_pos {
		( position.x - screen_viewport.x ) / screen_viewport.w,
		( position.y - screen_viewport.y ) / screen_viewport.h
	};

	//	Translate to screen-space position of range -1.0f:1.0f
	const Vec2 screen_space_pos {
		( normalized_screen_pos.x - 0.5f ) * 2.0f,
		( ( 1.0f - normalized_screen_pos.y ) - 0.5f ) * 2.0f
	};

	//	Add Z-axis to translate to Normalized Device Coordinates
	//	and to compute both near and far world-space locations
	const Vec3 proj_space_near_pos( screen_space_pos.x, screen_space_pos.y, 0.0f );
	const Vec3 proj_space_far_pos( screen_space_pos.x, screen_space_pos.y, 1.0f );

	//	Compute inverse view-projection matrix
	const Mtx4 inverse_matrix = ( _view_matrix * _projection_matrix ).inverse();

	//	Compute world-space locations
	const Vec3 world_space_near_pos = Vec3::transform_with_perspective_div( proj_space_near_pos, inverse_matrix );
	const Vec3 world_space_far_pos = Vec3::transform_with_perspective_div( proj_space_far_pos, inverse_matrix );

	return Ray( world_space_near_pos, world_space_far_pos - world_space_near_pos );
}

void Camera::look_at( const Vec3& target )
{
	transform->set_rotation(
		Quaternion::look_at(
			transform->location + _offset,
			target,
			Vec3::up
		)
	);
	//view_matrix = Mtx4::create_look_at( transform->location, target, Vec3::up );
	_is_view_matrix_dirty = true;
}

void Camera::set_offset( const Vec3& offset )
{
	_offset = offset;
	_is_view_matrix_dirty = true;
}

void Camera::reset()
{
	normalized_screen_viewport = { 0.0f, 0.0f, 1.0f, 1.0f };
	zoom = 1.0f;

	clip = { 0.0f, 0.0f, 0.0f, 0.0f };
	clip_enabled = false;
}

void Camera::set_active()
{
	_is_active = true;
}

bool Camera::is_active() const
{
	return _is_active;
}

Rect Camera::get_screen_viewport() const
{
	const Window* window = Engine::instance().get_window();
	const Vec2 window_size = window->get_size();

	return Rect {
		normalized_screen_viewport.x * window_size.x,
		normalized_screen_viewport.y * window_size.y,
		normalized_screen_viewport.w * window_size.x,
		normalized_screen_viewport.h * window_size.y
	};
}

void Camera::set_view_matrix( const Mtx4& matrix )
{
	_view_matrix = matrix;
	_is_view_matrix_dirty = false;

	//	Compute transform for external uses
	transform->get_matrix();
}

const Mtx4& Camera::get_view_matrix()
{
	if ( _is_view_matrix_dirty || transform->is_matrix_dirty() )
	{
		//  TODO: fix this for 2D
		const Vec3 origin = transform->location + _offset;
		set_view_matrix(
			Mtx4::create_look_at(
				origin,
				origin + transform->get_forward(),
				up_direction
			)
		);
	}

	return _view_matrix;
}

const Mtx4& Camera::get_projection_matrix() const
{
	return _projection_matrix;
}

void Camera::setup_vars()
{
	//	Listen to window updates
	Window* window = Engine::instance().get_window();
	window->on_size_changed.listen( &Camera::_on_window_resized, this );
}

void Camera::_on_window_resized( const Vec2& new_size, const Vec2& old_size )
{
	update_projection_from_settings();
}