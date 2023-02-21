#include "camera.h"

#include <suprengine/game.h>
#include <suprengine/ecs/entity.h>

using namespace suprengine;

bool Camera::try_register()
{
	Game* game = owner->get_game();
	if ( game->camera == nullptr )
	{
		game->camera = this;
		return true;
	}

	return false;
}

void Camera::setup_vars()
{
	Window* window = owner->get_game()->get_window();
	viewport_size.x = (float) window->get_width(), viewport_size.y = (float) window->get_height();
	viewport.w, viewport.h = viewport_size.x, viewport_size.y;  //  TODO: remove obsolete camera code
}

Camera::Camera( Entity* owner )
	: Component( owner )
{
	setup_vars();
	setup_simple_projection();
	try_register();
}

Camera::Camera( Entity* owner, float fov, float znear, float zfar )
	: Component( owner )
{
	setup_vars();
	setup_perspective( fov, znear, zfar );
	try_register();
}

void Camera::setup_simple_projection()
{
	projection_matrix = Mtx4::create_simple_view_projection(
		viewport_size.x,
		viewport_size.y
	);
}

void Camera::setup_perspective( float fov, float znear, float zfar )
{
	projection_matrix = Mtx4::create_perspective_fov( fov * math::DEG2RAD, viewport_size.x, viewport_size.y, znear, zfar );
}

void Camera::look_at( const Vec3& target )
{
	transform->set_rotation( 
		Quaternion::look_at(
			transform->location + offset, 
			target,
			Vec3::up
		) 
	);
	//view_matrix = Mtx4::create_look_at( transform->location, target, Vec3::up );
	//is_view_matrix_dirty = true;
}

void Camera::set_offset( const Vec3& offset )
{
	this->offset = offset;
	is_view_matrix_dirty = true;
}

void Camera::reset( float width, float height )
{
	viewport_size.x = width, viewport_size.y = height;
	viewport = { 0.0f, 0.0f, width, height };
	zoom = 1.0f;

	clip = { 0.0f, 0.0f, 0.0f, 0.0f };
	clip_enabled = false;
}

const Mtx4& Camera::get_view_matrix() 
{ 
	if ( is_view_matrix_dirty || transform->is_matrix_dirty )
	{
		//  TODO: fix this for 2D
		Vec3 origin = transform->location + offset;
		view_matrix = Mtx4::create_look_at( origin, origin + transform->get_forward(), Vec3::up );

		is_view_matrix_dirty = false;

		//  compute transform for external uses
		transform->get_matrix();
	}

	return view_matrix;
}
