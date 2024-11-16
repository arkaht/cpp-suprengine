#include "vec3.h"

#include <suprengine/vec4.h>
#include <suprengine/mtx4.h>
#include <suprengine/quaternion.h>

using namespace suprengine;

const Vec3 Vec3::zero( 0.0f, 0.0f, 0.0f );
const Vec3 Vec3::one( 1.0f, 1.0f, 1.0f );

const Vec3 Vec3::unit_x( 1.0f, 0.0f, 0.0f );
const Vec3 Vec3::unit_y( 0.0f, 1.0f, 0.0f );
const Vec3 Vec3::unit_z( 0.0f, 0.0f, 1.0f );

const Vec3 Vec3::forward( 1.0f, 0.0f, 0.0f );
const Vec3 Vec3::right( 0.0f, 1.0f, 0.0f );
const Vec3 Vec3::up( 0.0f, 0.0f, 1.0f );

const Vec3 Vec3::infinity( math::PLUS_INFINITY, math::PLUS_INFINITY, math::PLUS_INFINITY );

Vec3::Vec3( float value )
	: x( value ), y( value ), z( value )
{}

Vec3::Vec3( float x, float y, float z )
	: x( x ), y( y ), z( z )
{}

Vec3::Vec3( const Vec2& vec, float z )
	: x( vec.x ), y( vec.y ), z( z )
{}

Vec3::Vec3( const Vec4& vec )
	: x( vec.x ), y( vec.y ), z( vec.z )
{}

float Vec3::length_sqr() const
{
	return x * x + y * y + z * z;
}

float Vec3::length() const
{
	return math::sqrt( length_sqr() );
}

float Vec3::length2d_sqr() const
{
	return x * x + y * y;
}

float Vec3::length2d() const
{
	return math::sqrt( length2d_sqr() );
}

void Vec3::normalize()
{
	float len = length();
	if ( len == 0.0f ) return;

	x /= len;
	y /= len;
	z /= len;
}

void Vec3::normalize2d()
{
	float len = length2d();
	if ( len == 0.0f ) return;

	x /= len;
	y /= len;
	z = 0.0f;
}

Vec3 Vec3::normalized() const
{
	Vec3 value( x, y, z );
	value.normalize();
	return value;
}

std::string Vec3::to_string() const
{
	return "x=" + std::to_string( x )
		+ ";y=" + std::to_string( y )
		+ ";z=" + std::to_string( z );
}

const float* Vec3::get_as_float_ptr() const
{
	return reinterpret_cast<const float*>( &x );
}

float Vec3::distance( const Vec3& from, const Vec3& to )
{
	return ( from - to ).length();
}

float Vec3::distance_sqr( const Vec3& from, const Vec3& to )
{
	return ( from - to ).length_sqr();
}

float Vec3::distance2d( const Vec3& from, const Vec3& to )
{
	return ( from - to ).length2d();
}

float Vec3::distance2d_sqr( const Vec3& from, const Vec3& to )
{
	return ( from - to ).length2d_sqr();
}

Vec3 Vec3::world_to_grid( const Vec3& value, float grid_size )
{
	return Vec3 {
		math::floor( value.x / grid_size ),
		math::floor( value.y / grid_size ),
		math::floor( value.z / grid_size ),
	};
}

Vec3 Vec3::snap_to_grid( const Vec3& value, float grid_size )
{
	return Vec3
	{
		math::snap_to_grid( value.x, grid_size ),
		math::snap_to_grid( value.y, grid_size ),
		math::snap_to_grid( value.z, grid_size ),
	};
}

Vec3 Vec3::snap_to_grid( const Vec3& value, const Vec3& grid_size )
{
	return Vec3
	{
		math::snap_to_grid( value.x, grid_size.x ),
		math::snap_to_grid( value.y, grid_size.y ),
		math::snap_to_grid( value.z, grid_size.z ),
	};
}

Vec3 Vec3::clamp( const Vec3& value, const Vec3& min, const Vec3& max )
{
	return Vec3 {
		math::clamp( value.x, min.x, max.x ),
		math::clamp( value.y, min.y, max.y ),
		math::clamp( value.z, min.z, max.z ),
	};
}

Vec3 Vec3::round( const Vec3& value )
{
	return Vec3 {
		math::round( value.x ),
		math::round( value.y ),
		math::round( value.z ),
	};
}

Vec3 Vec3::normalize( const Vec3& vec )
{
	Vec3 temp = vec;
	temp.normalize();
	return temp;
}

float Vec3::dot( const Vec3& a, const Vec3& b )
{
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

Vec3 Vec3::cross( const Vec3& a, const Vec3& b )
{
	Vec3 temp;
	temp.x = a.y * b.z - a.z * b.y;
	temp.y = a.z * b.x - a.x * b.z;
	temp.z = a.x * b.y - a.y * b.x;
	return temp;
}

Vec3 Vec3::lerp( const Vec3& a, const Vec3& b, float f )
{
	return Vec3 {
		math::lerp( a.x, b.x, f ),
		math::lerp( a.y, b.y, f ),
		math::lerp( a.z, b.z, f ),
	};
}

Vec3 Vec3::reflect( const Vec3& value, const Vec3& n )
{
	return value - 2.0f * Vec3::dot( value, n ) * n;
}

Vec3 Vec3::approach( const Vec3& current, const Vec3& target, float delta )
{
	return Vec3 {
		math::approach( current.x, target.x, delta ),
		math::approach( current.y, target.y, delta ),
		math::approach( current.z, target.z, delta )
	};
}

Vec3 Vec3::sqrt( const Vec3& current )
{
	return Vec3 {
		math::sqrt( current.x ),
		math::sqrt( current.y ),
		math::sqrt( current.z )
	};
}

Vec3 Vec3::transform( const Vec3& vec, const Mtx4& matrix, float w )
{
	Vec4 vec4 = Vec4( vec, w ) * matrix;
	return Vec3( vec4 );
}

Vec3 Vec3::transform_with_perspective_div( const Vec3& vec, const Mtx4& matrix, float w )
{
	Vec4 vec4 = Vec4( vec, w ) * matrix;

	//	NOTE: It is important to check for a literal zero and not for an approximation
	//	like math::near_zero does because the value can be so little that it may returns
	//	false positive when you really need that division to occurs.
	//	Trust me, it would save you days of investigation.
	//
	//	It happened to me when I was working on the Camera::viewport_to_world function
	//	where the near world-space location was computed correctly but the far location
	//	wasn't. It was due to the use of math::near_zero function which checked for a
	//	precision up to 10^-3 where my vec4.w was computed with a 10^-5 precision, hence
	//	the division was never occuring and the returned location was incorrect.
	//
	//	I decided to directly check for zero instead of making the approximation precision
	//	greater because, if I did so, it would only behave correctly until you set a too great
	//	value for the Z-Far of the camera.
	if ( vec4.w != 0.0f )
	{
		float inverse_w = 1.0f / vec4.w;
		vec4.x *= inverse_w;
		vec4.y *= inverse_w;
		vec4.z *= inverse_w;
	}

	return Vec3( vec4 );
}

Vec3 Vec3::transform( const Vec3& value, const Quaternion& q )
{
	// value + 2.0*cross(q.xyz, cross(q.xyz,value) + q.w*value);
	Vec3 qv( q.x, q.y, q.z );
	Vec3 retVal = value;
	retVal += 2.0f * Vec3::cross( qv, Vec3::cross( qv, value ) + q.w * value );
	return retVal;
}