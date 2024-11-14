#include "vec4.h"
#include "vec4.h"
#include "vec4.h"

#include <suprengine/vec3.h>
#include <suprengine/mtx4.h>

using namespace suprengine;

Vec4::Vec4( float value )
	: x( value ), y( value ), z( value ), w( value )
{}

Vec4::Vec4( float x, float y, float z, float w )
	: x( x ), y( y ), z( z ), w( w )
{}

Vec4::Vec4( const Vec3& vec, float w )
	: x( vec.x ), y( vec.y ), z( vec.z ), w( w )
{}

Vec4 Vec4::operator*( const Mtx4& mtx )
{
	//	NOTE: For some reasons, I can't make a copy Vec4 of this using *=
	//	and having the same result, so here we go...
	return Vec4 {
		x * mtx[0][0] + y * mtx[1][0] + z * mtx[2][0] + w * mtx[3][0],
		x * mtx[0][1] + y * mtx[1][1] + z * mtx[2][1] + w * mtx[3][1],
		x * mtx[0][2] + y * mtx[1][2] + z * mtx[2][2] + w * mtx[3][2],
		x * mtx[0][3] + y * mtx[1][3] + z * mtx[2][3] + w * mtx[3][3]
	};
}

Vec4 Vec4::operator/( float value )
{
	Vec4 copy = *this;
	copy /= value;
	return copy;
}

Vec4& Vec4::operator*=( const Mtx4& mtx )
{
	x = x * mtx[0][0] + y * mtx[1][0] + z * mtx[2][0] + w * mtx[3][0];
	y = x * mtx[0][1] + y * mtx[1][1] + z * mtx[2][1] + w * mtx[3][1];
	z = x * mtx[0][2] + y * mtx[1][2] + z * mtx[2][2] + w * mtx[3][2];
	w = x * mtx[0][3] + y * mtx[1][3] + z * mtx[2][3] + w * mtx[3][3];

	return *this;
}

Vec4& Vec4::operator*=( float value )
{
	x *= value;
	y *= value;
	z *= value;
	w *= value;

	return *this;
}

Vec4& Vec4::operator/=( float value )
{
	value = 1.0f / value;

	x *= value;
	y *= value;
	z *= value;
	w *= value;

	return *this;
}

std::string Vec4::to_string() const
{
	return "x=" + std::to_string( x )
		+ ";y=" + std::to_string( y )
		+ ";z=" + std::to_string( z )
		+ ";w=" + std::to_string( w );
}
