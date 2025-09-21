#include "transform.h"

using namespace suprengine;

Rect Transform::get_rect( const Rect& rect )
{
	return Rect {
		location.x + rect.x * scale.x,
		location.y + rect.y * scale.y,
		rect.w * scale.x,
		rect.h * scale.y
	};
}

void Transform::set_location( const Vec2& vec )
{
	location.x = vec.x, location.y = vec.y, location.z = 0.0f;
	_is_matrix_dirty = true;
}

void Transform::set_location( const Vec3& vec )
{
	location = vec;
	_is_matrix_dirty = true;
}

void Transform::set_rotation( const Quaternion& quat )
{
	rotation = quat;
	_is_matrix_dirty = true;
}

void Transform::set_scale( const Vec2& vec )
{
	scale.x = vec.x, scale.y = vec.y, scale.z = 1.0f;
	_is_matrix_dirty = true;
}

void Transform::set_scale( const Vec3& vec )
{
	scale = vec;
	_is_matrix_dirty = true;
}

void Transform::look_at( const Vec3& target )
{
	rotation = Quaternion::look_at(
		location,
		target,
		Vec3::up
	);
	_is_matrix_dirty = true;
}

Vec3 Transform::get_right() const 
{ 
	return Vec3::transform( Vec3::right, rotation ); 
}

Vec3 Transform::get_forward() const 
{ 
	return Vec3::transform( Vec3::forward, rotation ); 
}

Vec3 Transform::get_up() const 
{ 
	return Vec3::transform( Vec3::up, rotation ); 
}

const Mtx4& Transform::get_matrix()
{
	if ( _is_matrix_dirty )
	{
		_matrix = Mtx4::create_from_transform( scale, rotation, location );
		_is_matrix_dirty = false;
	}

	return _matrix;
}

bool Transform::is_matrix_dirty() const
{
	return _is_matrix_dirty;
}