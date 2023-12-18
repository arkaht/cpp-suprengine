#include "camera.h"

#include <suprengine/game.h>
#include <suprengine/entity.h>

using namespace suprengine;

Camera::Camera( Entity* owner )
	: Component( owner )
{
	setup_vars();
	setup_simple_projection();
}

Camera::Camera( Entity* owner, const CameraProjectionSettings& projection_settings )
	: projection_settings( projection_settings ),
	  Component( owner )
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

Vec3 Camera::world_to_viewport( const Vec3& location )
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
	Engine* game = owner->get_game();
	game->camera = this;
}

bool Camera::is_active()
{
	return owner->get_game()->camera == this;
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
	if ( _is_view_matrix_dirty || transform->is_matrix_dirty )
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
	Window* _window = owner->get_game()->get_window();

	//  listen to window updates
	_window->on_size_changed.listen( 
		"suprengine::camera" + std::to_string( owner->get_unique_id() ), 
		[&]( const Vec2& size ) {
			_viewport_size = size;
			update_projection_from_settings();
		}
	);

	_viewport_size = _window->get_size();
	viewport.w, viewport.h = _viewport_size.x, _viewport_size.y;  //  TODO: remove obsolete camera code
}