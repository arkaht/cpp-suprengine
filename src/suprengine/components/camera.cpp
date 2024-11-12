#include "camera.h"

#include <suprengine/vec4.h>

#include <suprengine/game.h>
#include <suprengine/entity.h>

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

	//  pass location to view & projection matrices
	pos = Vec3::transform( pos, _view_matrix );
	pos = Vec3::transform( pos, _projection_matrix );

	//  normalize by Z-axis
	pos.x /= pos.z;
	pos.y /= pos.z;

	//  remap [-1.0f; 1.0f] to [0.0f; 1.0f]
	pos.x = ( pos.x + 1.0f ) * 0.5f;
	pos.y = ( pos.y + 1.0f ) * 0.5f;
	//printf( "x=%f; y=%f; z=%f\n", pos.x, pos.y, pos.z );

	//  to viewport
	pos.x *= _viewport_size.x;
	pos.y *= _viewport_size.y;

	return pos;
}

Vec3 Camera::viewport_to_world( const Vec2& location ) const
{
	//  https://stackoverflow.com/a/56348846

	Vec4 pos {
		( location.x - _viewport_size.x * 0.5f ) / _viewport_size.x * 2.0f,
		( location.y - _viewport_size.y * 0.5f ) / _viewport_size.y * 2.0f,
		-1.0f,
		1.0f
	};
	printf( "Viewport: %s\n", *pos.to_string() );

	Mtx4 inverse_matrix = _projection_matrix * _view_matrix;
	inverse_matrix.invert();

	pos = pos * inverse_matrix;
	pos /= pos.w;
	//pos = Vec3::transform( pos, inverse_projection_matrix );

	//pos -= transform->location;

	return Vec3( pos.x, pos.y, pos.z );
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

	//  compute transform for external uses
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

	//  listen to window updates
	window->on_size_changed.listen( 
		"suprengine::camera" + std::to_string( get_owner()->get_unique_id() ), 
		[&]( const Vec2& new_size, const Vec2& old_size ) {
			_viewport_size = new_size;
			update_projection_from_settings();
		}
	);

	_viewport_size = window->get_size();
	viewport.w, viewport.h = _viewport_size.x, _viewport_size.y;  //  TODO: remove obsolete camera code
}