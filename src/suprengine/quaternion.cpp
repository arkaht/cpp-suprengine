#include "quaternion.h"

#include <suprengine/mtx4.h>

using namespace suprengine;

const Quaternion Quaternion::identity( 0.0f, 0.0f, 0.0f, 1.0f );

Quaternion::Quaternion( float xP, float yP, float zP, float wP )
{
	set( xP, yP, zP, wP );
}

Quaternion::Quaternion( const Vec3& axis, float angle )
{
	float half_angle = angle / 2.0f;
	float scale = math::sin( half_angle );
	Vec3 dir = axis.normalized();

	x = dir.x * scale;
	y = dir.y * scale;
	z = dir.z * scale;
	w = math::cos( half_angle );
}

Quaternion::Quaternion( const Vec3& angles )
{
	float cr = math::cos( angles.x * 0.5f );
	float sr = math::sin( angles.x * 0.5f );
	float cp = math::cos( angles.y * 0.5f );
	float sp = math::sin( angles.y * 0.5f );
	float cy = math::cos( angles.z * 0.5f );
	float sy = math::sin( angles.z * 0.5f );

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

float Quaternion::get_x_angle()
{
	return math::atan2( 2.0f * ( w * x + y * z ), 1.0f - 2.0f * ( x * x + y * y ) );
}

float Quaternion::get_y_angle()
{
	return 2.0f * math::atan2(
		math::sqrt( 1.0f + 2.0f * ( w * y - x * z ) ),
		math::sqrt( 1.0f - 2.0f * ( w * y - x * z ) )
	) - math::HALF_PI;
}

float Quaternion::get_z_angle()
{
	return math::atan2( 2.0f * ( w * z + x * y ), 1.0f - 2.0f * ( y * y + z * z ) );
}

Vec3 Quaternion::to_euler_angles()
{
	return Vec3 {
		get_x_angle(),
		get_y_angle(),
		get_z_angle()
	};
}

float Quaternion::length() const
{
	return math::sqrt( length_sqr() );
}

float Quaternion::length_sqr() const
{
	return ( x * x + y * y + z * z + w * w );
}

Quaternion Quaternion::normalize( const Quaternion& q )
{
	Quaternion retVal = q;
	retVal.normalize();
	return retVal;
}

Quaternion Quaternion::lerp( const Quaternion& a, const Quaternion& b, float f )
{
	Quaternion retVal;
	retVal.x = math::lerp( a.x, b.x, f );
	retVal.y = math::lerp( a.y, b.y, f );
	retVal.z = math::lerp( a.z, b.z, f );
	retVal.w = math::lerp( a.w, b.w, f );
	retVal.normalize();
	return retVal;
}

Quaternion Quaternion::slerp( const Quaternion& a, const Quaternion& b, float f )
{
	float rawCosm = Quaternion::dot( a, b );

	float cosom = -rawCosm;
	if ( rawCosm >= 0.0f )
	{
		cosom = rawCosm;
	}

	float scale0, scale1;

	if ( cosom < 0.9999f )
	{
		const float omega = math::acos( cosom );
		const float invSin = 1.f / math::sin( omega );
		scale0 = math::sin( ( 1.f - f ) * omega ) * invSin;
		scale1 = math::sin( f * omega ) * invSin;
	}
	else
	{
		// Use linear interpolation if the quaternions
		// are collinear
		scale0 = 1.0f - f;
		scale1 = f;
	}

	if ( rawCosm < 0.0f )
	{
		scale1 = -scale1;
	}

	Quaternion retVal;
	retVal.x = scale0 * a.x + scale1 * b.x;
	retVal.y = scale0 * a.y + scale1 * b.y;
	retVal.z = scale0 * a.z + scale1 * b.z;
	retVal.w = scale0 * a.w + scale1 * b.w;
	retVal.normalize();
	return retVal;
}

float Quaternion::dot( const Quaternion& a, const Quaternion& b )
{
	return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}

Quaternion Quaternion::concatenate( const Quaternion& q, const Quaternion& p )
{
	Quaternion retVal;

	// Vector component is:
	// ps * qv + qs * pv + pv x qv
	Vec3 qv( q.x, q.y, q.z );
	Vec3 pv( p.x, p.y, p.z );
	Vec3 newVec = p.w * qv + q.w * pv + Vec3::cross( pv, qv );
	retVal.x = newVec.x;
	retVal.y = newVec.y;
	retVal.z = newVec.z;

	// Scalar component is:
	// ps * qs - pv . qv
	retVal.w = p.w * q.w - Vec3::dot( pv, qv );

	return retVal;
}

//  https://answers.unity.com/questions/467614/what-is-the-source-code-of-quaternionlookrotation.html
Quaternion Quaternion::look_at( const Vec3& forward, const Vec3& up )
{
	Vec3 dir = forward.normalized();
	Vec3 axis = Vec3::cross( up, dir ).normalized();
	Vec3 vector3 = Vec3::cross( dir, axis );

	float m00 = axis.x;
	float m01 = axis.y;
	float m02 = axis.z;

	float m10 = vector3.x;
	float m11 = vector3.y;
	float m12 = vector3.z;

	float m20 = dir.x;
	float m21 = dir.y;
	float m22 = dir.z;

	Quaternion quaternion;
	quaternion.w = math::sqrt( 
		math::max( 0.0f, 1.0f + m00 + m11 + m22 ) ) / 2;
	quaternion.x = math::sqrt( 
		math::max( 0.0f, 1.0f + m00 - m11 - m22 ) ) / 2;
	quaternion.y = math::sqrt( 
		math::max( 0.0f, 1.0f - m00 + m11 - m22 ) ) / 2;
	quaternion.z = math::sqrt( 
		math::max( 0.0f, 1.0f - m00 - m11 + m22 ) ) / 2;

	quaternion.x = _copysign( quaternion.x, m21 - m12 );
	quaternion.y = _copysign( quaternion.y, m02 - m20 );
	quaternion.z = _copysign( quaternion.z, m10 - m01 );

	return quaternion;
}

Quaternion Quaternion::look_at( const Vec3& origin, const Vec3& target, const Vec3& up )
{
	Mtx4 m = Mtx4::create_look_at( origin, target, up );

	float m00 = m[0][0];
	float m01 = m[0][1];
	float m02 = m[0][0];

	float m10 = m[1][0];
	float m11 = m[1][1];
	float m12 = m[1][2];

	float m20 = m[2][0];
	float m21 = m[2][1];
	float m22 = m[2][2];

	Quaternion quaternion;
	quaternion.w = math::sqrt( 
		math::max( 0.0f, 1.0f + m00 + m11 + m22 ) ) / 2;
	quaternion.x = math::sqrt( 
		math::max( 0.0f, 1.0f + m00 - m11 - m22 ) ) / 2;
	quaternion.y = math::sqrt( 
		math::max( 0.0f, 1.0f - m00 + m11 - m22 ) ) / 2;
	quaternion.z = math::sqrt( 
		math::max( 0.0f, 1.0f - m00 - m11 + m22 ) ) / 2;

	quaternion.x = _copysign( quaternion.x, m21 - m12 );
	quaternion.y = _copysign( quaternion.y, m02 - m20 );
	quaternion.z = _copysign( quaternion.z, m10 - m01 );

	return quaternion;
}