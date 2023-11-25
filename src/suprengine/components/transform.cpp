#include "transform.h"

using namespace suprengine;

Rect Transform::get_rect( const Rect& rect )
{
	return {
		location.x + rect.x * scale.x,
		location.y + rect.y * scale.y,
		rect.w * scale.x,
		rect.h * scale.y
	};
}

void Transform::set_location( const Vec2& vec )
{
	location.x = vec.x, location.y = vec.y, location.z = 0.0f;
	is_matrix_dirty = true;
}

void Transform::set_location( const Vec3& vec )
{
	location = vec;
	is_matrix_dirty = true;
}

void Transform::set_rotation( const Quaternion& quat )
{
	rotation = quat;
	is_matrix_dirty = true;
}

void Transform::set_scale( const Vec2& vec )
{
	scale.x = vec.x, scale.y = vec.y, scale.z = 1.0f;
	is_matrix_dirty = true;
}

void Transform::set_scale( const Vec3& vec )
{
	scale = vec;
	is_matrix_dirty = true;
}

void Transform::look_at( const Vec3& target )
{
	rotation = Quaternion::look_at(
		location,
		target,
		Vec3::up
	);

	is_matrix_dirty = true;
}

const Mtx4& Transform::get_matrix()
{
	if ( is_matrix_dirty )
	{
		matrix = Mtx4::create_from_transform( scale, rotation, location );
		is_matrix_dirty = false;
	}

	return matrix;
}
