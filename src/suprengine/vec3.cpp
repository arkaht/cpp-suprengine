#include "vec3.h"
#include "mtx4.h"
#include "quaternion.h"

using namespace suprengine;


const Vec3 
	Vec3::zero( 0.0f, 0.0f, 0.0f ), 
	Vec3::one( 1.0f, 1.0f, 1.0f );
const Vec3 
	Vec3::unit_x( 1.0f, 0.0f, 0.0f ), 
	Vec3::unit_y( 0.0f, 1.0f, 0.0f ), 
	Vec3::unit_z( 0.0f, 0.0f, 1.0f );
const Vec3 
	Vec3::forward( 1.0f, 0.0f, 0.0f ), 
	Vec3::right( 0.0f, 1.0f, 0.0f ),
	Vec3::up( 0.0f, 0.0f, 1.0f );
const Vec3 
	Vec3::infinity( math::PLUS_INFINITY, math::PLUS_INFINITY, math::PLUS_INFINITY );


Vec3::Vec3( float value ) 
	: x( value ), y( value ), z( value ) 
{}

Vec3::Vec3( const Vec2& vec ) 
	: x( vec.x ), y( vec.y ), z( 0.0f ) 
{}

Vec3::Vec3( const Vec2& vec, float z ) 
	: x( vec.x ), y( vec.y ), z( z ) 
{}

Vec3::Vec3( float x, float y, float z )
	: x( x ), y( y ), z( z )
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
	Vec3 v( x, y, z );
	v.normalize();
	return v;
}

std::string Vec3::to_string() const
{
	return "x="  + std::to_string( x ) 
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

Vec3 Vec3::world_to_grid( const Vec3& v, float grid_size )
{
	return Vec3 
	{
		math::floor( v.x / grid_size ),
		math::floor( v.y / grid_size ),
		math::floor( v.z / grid_size ),
	};
}

Vec3 Vec3::snap_to_grid( const Vec3& v, float grid_size )
{
	return Vec3 
	{
		math::snap_to_grid( v.x, grid_size ),
		math::snap_to_grid( v.y, grid_size ),
		math::snap_to_grid( v.z, grid_size ),
	};
}

Vec3 Vec3::snap_to_grid( const Vec3& v, const Vec3& grid_size )
{
	return Vec3 
	{
		math::snap_to_grid( v.x, grid_size.x ),
		math::snap_to_grid( v.y, grid_size.y ),
		math::snap_to_grid( v.z, grid_size.z ),
	};
}

Vec3 Vec3::round( const Vec3 & v )
{
	return Vec3 {
		math::round( v.x ),
		math::round( v.y ),
		math::round( v.z ),
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

Vec3 Vec3::reflect( const Vec3& v, const Vec3& n )
{
	return v - 2.0f * Vec3::dot( v, n ) * n;
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