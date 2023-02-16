#include "camera.hpp"

using namespace suprengine;

const Mtx4& Camera::get_matrix()
{
	if ( is_matrix_dirty )
	{
		matrix = Mtx4::create_translation( viewport.x, -viewport.y, 0.0f )//;
			   * Mtx4::create_scale( zoom, zoom, 1.0f )
			   * Mtx4::create_translation( viewport.w, -viewport.h, 0.0f );

		printf( "hello %f x:%f y:%f\n", zoom, viewport.x, viewport.y );
		is_matrix_dirty = false;
	}

	return matrix;
}
