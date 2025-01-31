#include "camera.h"

#include <suprengine/core/engine.h>

#include <suprengine/math/vec4.h>

#include <suprengine/components/transform.h>

using namespace suprengine;

Camera::Camera()
{}

Camera::Camera( const CameraProjectionSettings& projection_settings )
	: projection_settings( projection_settings )
{}

void Camera::setup()
{
	setup_vars();
	update_projection_from_settings();
}

void Camera::setup_simple_projection()
{
	_projection_matrix = Mtx4::create_simple_view_projection(
		_viewport_size.x,
		_viewport_size.y
	);
}

void Camera::setup_perspective( float fov, float znear, float zfar )
{
	_projection_matrix = Mtx4::create_perspective_fov(
		fov * math::DEG2RAD,
		_viewport_size.x, _viewport_size.y,
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

void Camera::set_fov( float fov )
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
	pos.x *= _viewport_size.x;
	pos.y *= _viewport_size.y;

	return pos;
}

Ray Camera::viewport_to_world( const Vec2& position ) const
{
	//	Normalize the viewport-space position of range: 0.0f:1.0f
	const Vec2 normalized_screen_pos {
		position.x / _viewport_size.x,
		position.y / _viewport_size.y
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

void Camera::reset( float width, float height )
{
	_viewport_size.x = width, _viewport_size.y = height;
	viewport = { 0.0f, 0.0f, width, height };
	zoom = 1.0f;

	clip = { 0.0f, 0.0f, 0.0f, 0.0f };
	clip_enabled = false;
}

void Camera::activate()
{
	auto& engine = Engine::instance();
	engine.camera = this;
}

bool Camera::is_active()
{
	auto& engine = Engine::instance();
	return engine.camera == this;
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
		Vec3 origin = transform->location + _offset;
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

const Mtx4& Camera::get_projection_matrix()
{
	return _projection_matrix;
}

void Camera::setup_vars()
{
	auto window = Engine::instance().get_window();

	//	Listen to window updates
	window->on_size_changed.listen( &Camera::_on_window_resized, this );

	_viewport_size = window->get_size();
	viewport.w, viewport.h = _viewport_size.x, _viewport_size.y;  //  TODO: Remove obsolete camera code
}

void Camera::_on_window_resized( const Vec2& new_size, const Vec2& old_size )
{
	_viewport_size = new_size;
	update_projection_from_settings();
}