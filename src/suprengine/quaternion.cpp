#include "quaternion.h"

using namespace suprengine;

const Quaternion Quaternion::identity( 0.0f, 0.0f, 0.0f, 1.0f );

Quaternion::Quaternion( float xP, float yP, float zP, float wP )
{
	set( xP, yP, zP, wP );
}

Quaternion::Quaternion( const Vec3& axis, float angle )
{
	float scalar = math::sin( angle / 2.0f );
	x = axis.x * scalar;
	y = axis.y * scalar;
	z = axis.z * scalar;
	w = math::cos( angle / 2.0f );
}

Quaternion::Quaternion( const Vec3& angles )
{
	float cr = math::cos( angles.x * 0.5 );
	float sr = math::sin( angles.x * 0.5 );
	float cp = math::cos( angles.y * 0.5 );
	float sp = math::sin( angles.y * 0.5 );
	float cy = math::cos( angles.z * 0.5 );
	float sy = math::sin( angles.z * 0.5 );

	w = cr * cp * cy + sr * sp * sy;
	x = sr * cp * cy - cr * sp * sy;
	y = cr * sp * cy + sr * cp * sy;
	z = cr * cp * sy - sr * sp * cy;
}

void Quaternion::set( float inX, float inY, float inZ, float inW )
{
	x = inX;
	y = inY;
	z = inZ;
	w = inW;
}

void Quaternion::conjugate()
{
	x *= -1.0f;
	y *= -1.0f;
	z *= -1.0f;
}

void Quaternion::normalize()
{
	float len = length();
	x /= len;
	y /= len;
	z /= len;
	w /= len;
}