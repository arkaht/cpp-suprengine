#include "vec3.h"
#include "mtx4.h"
#include "quaternion.h"

using namespace suprengine;

const Vec3 Vec3::zero( 0.0f, 0.0f, 0.0f ), Vec3::one( 1.0f, 1.0f, 1.0f );
const Vec3 Vec3::unit_x( 1.0f, 0.0f, 0.0f ), Vec3::unit_y( 0.0f, 1.0f, 0.0f ), Vec3::unit_z( 0.0f, 0.0f, 1.0f );
const Vec3 Vec3::forward( 1.0f, 0.0f, 0.0f ), Vec3::right( 0.0f, 1.0f, 0.0f ), Vec3::up( 0.0f, 0.0f, 1.0f );
const Vec3 Vec3::neg_unit_x( -1.0f, 0.0f, 0.0f ), Vec3::neg_unit_y( 0.0f, -1.0f, 0.0f ), Vec3::neg_unit_z( 0.0f, 0.0f, -1.0f );
const Vec3 Vec3::infinity( math::PLUS_INFINITY, math::PLUS_INFINITY, math::PLUS_INFINITY );
const Vec3 Vec3::neg_infinity( math::PLUS_INFINITY, math::PLUS_INFINITY, math::PLUS_INFINITY );

void Vec3::set( float xP, float yP, float zP )
{
	x = xP;
	y = yP;
	z = zP;
}

float Vec3::length_sqr() const
{
	return x * x + y * y + z * z;
}

float Vec3::length() const
{
	float len = length_sqr();
	if ( len == 0.0f ) return 0.0f;

	return math::sqrt( len );
}

void Vec3::normalize()
{
	float len = length();
	if ( len == 0.0f ) return;

	x /= len;
	y /= len;
	z /= len;
}

Vec3 Vec3::normalized() const
{
	Vec3 v( x, y, z );
	v.normalize();
	return v;
}

Vec3 Vec3::transform( const Vec3& vec, const Mtx4& mat, float w )
{
	Vec3 retVal;
	retVal.x = vec.x * mat.mat[0][0] + vec.y * mat.mat[1][0] +
		vec.z * mat.mat[2][0] + w * mat.mat[3][0];
	retVal.y = vec.x * mat.mat[0][1] + vec.y * mat.mat[1][1] +
		vec.z * mat.mat[2][1] + w * mat.mat[3][1];
	retVal.z = vec.x * mat.mat[0][2] + vec.y * mat.mat[1][2] +
		vec.z * mat.mat[2][2] + w * mat.mat[3][2];
	//ignore w since we aren't returning a new value for it...
	return retVal;
}

Vec3 Vec3::transform_with_perspective_div( const Vec3& vec, const Mtx4& mat, float w )
{
	Vec3 retVal;
	retVal.x = vec.x * mat.mat[0][0] + vec.y * mat.mat[1][0] +
		vec.z * mat.mat[2][0] + w * mat.mat[3][0];
	retVal.y = vec.x * mat.mat[0][1] + vec.y * mat.mat[1][1] +
		vec.z * mat.mat[2][1] + w * mat.mat[3][1];
	retVal.z = vec.x * mat.mat[0][2] + vec.y * mat.mat[1][2] +
		vec.z * mat.mat[2][2] + w * mat.mat[3][2];
	float transformedW = vec.x * mat.mat[0][3] + vec.y * mat.mat[1][3] +
		vec.z * mat.mat[2][3] + w * mat.mat[3][3];

	if ( !math::near_zero( math::abs( transformedW ) ) )
	{
		transformedW = 1.0f / transformedW;
		retVal *= transformedW;
	}

	return retVal;
}

Vec3 Vec3::transform( const Vec3& v, const Quaternion& q )
{
	// v + 2.0*cross(q.xyz, cross(q.xyz,v) + q.w*v);
	Vec3 qv( q.x, q.y, q.z );
	Vec3 retVal = v;
	retVal += 2.0f * Vec3::cross( qv, Vec3::cross( qv, v ) + q.w * v );
	return retVal;
}