#include "quaternion.h"

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


    float num8 = ( m00 + m11 ) + m22;
    Quaternion quaternion;
    if ( num8 > 0.0f )
    {
        float num = math::sqrt( num8 + 1.0f );
        quaternion.w = num * 0.5f;
        num = 0.5f / num;
        quaternion.x = ( m12 - m21 ) * num;
        quaternion.y = ( m20 - m02 ) * num;
        quaternion.z = ( m01 - m10 ) * num;
        return quaternion;
    }
    if ( ( m00 >= m11 ) && ( m00 >= m22 ) )
    {
        float num7 = math::sqrt( ( ( 1.0f + m00 ) - m11 ) - m22 );
        float num4 = 0.5f / num7;
        quaternion.x = 0.5f * num7;
        quaternion.y = ( m01 + m10 ) * num4;
        quaternion.z = ( m02 + m20 ) * num4;
        quaternion.w = ( m12 - m21 ) * num4;
        return quaternion;
    }
    if ( m11 > m22 )
    {
        float num6 = math::sqrt( ( ( 1.0f + m11 ) - m00 ) - m22 );
        float num3 = 0.5f / num6;
        quaternion.x = ( m10 + m01 ) * num3;
        quaternion.y = 0.5f * num6;
        quaternion.z = ( m21 + m12 ) * num3;
        quaternion.w = ( m20 - m02 ) * num3;
        return quaternion;
    }
    float num5 = math::sqrt( ( ( 1.0f + m22 ) - m00 ) - m11 );
    float num2 = 0.5f / num5;
    quaternion.x = ( m20 + m02 ) * num2;
    quaternion.y = ( m21 + m12 ) * num2;
    quaternion.z = 0.5f * num5;
    quaternion.w = ( m01 - m10 ) * num2;
    return quaternion;
}
