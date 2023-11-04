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

Camera::Camera( Entity* owner, float fov, float znear, float zfar )
	: Component( owner )
{
	setup_vars();
	setup_perspective( fov, znear, zfar );
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
	_projection_matrix = Mtx4::create_perspective_fov( fov * math::DEG2RAD, _viewport_size.x, _viewport_size.y, znear, zfar );
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
	Game* game = owner->get_game();
	game->camera = this;
}

const Mtx4& Camera::get_view_matrix() 
{ 
	if ( _is_view_matrix_dirty || transform->is_matrix_dirty )
	{
		//  TODO: fix this for 2D
		Vec3 origin = transform->location + _offset;
		_view_matrix = Mtx4::create_look_at( 
			origin, 
			origin + transform->get_forward(), 
			Vec3::up 
		);

		_is_view_matrix_dirty = false;

		//  compute transform for external uses
		transform->get_matrix();
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
	_viewport_size.x = (float) _window->get_width(), _viewport_size.y = (float) _window->get_height();
	viewport.w, viewport.h = _viewport_size.x, _viewport_size.y;  //  TODO: remove obsolete camera code
}