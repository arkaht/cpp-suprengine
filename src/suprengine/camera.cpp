#include "camera.h"

using namespace suprengine;

void Camera::setup_simple_projection()
{
	projection_matrix = Mtx4::create_simple_view_projection( viewport.w, -viewport.h );
}

void Camera::setup_perspective( float fov, float znear, float zfar )
{
	projection_matrix = Mtx4::create_perspective_fov( fov * math::DEG2RAD, viewport.w, viewport.h, znear, zfar );
}

void Camera::look_at( const Vec3& target )
{
	view_matrix = Mtx4::create_look_at( location, target, Vec3::up );
}

const Mtx4& Camera::get_view_matrix()
{
	if ( is_view_matrix_dirty )
	{
		//  TODO: fix this
		view_matrix = Mtx4::create_translation( viewport.x, viewport.y, 0.0f )//;
					* Mtx4::create_scale( zoom, zoom, 1.0f )
					* Mtx4::create_translation( viewport.w, viewport.h, 0.0f );

		printf( "hello %f x:%f y:%f\n", zoom, viewport.x, viewport.y );
		is_view_matrix_dirty = false;
	}

	return view_matrix;
}
