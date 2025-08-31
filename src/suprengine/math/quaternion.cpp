#include "quaternion.h"

#include <suprengine/math/math.h>

using namespace suprengine;

const Quaternion Quaternion::identity {};

Quaternion::Quaternion( const float x, const float y, const float z, const float w )
	: x( x ), y( y ), z( z ), w( w )
{}

Quaternion::Quaternion( const Vec3& axis, const float angle )
{
	const float half_angle = angle / 2.0f;
	const float scale	   = math::sin( half_angle );
	const Vec3 dir = axis.normalized();

	x = dir.x * scale;
	y = dir.y * scale;
	z = dir.z * scale;
	w = math::cos( half_angle );
}

Quaternion::Quaternion( const RadAngles& angles )
{
	const float pitch = angles.p * 0.5f;
	const float yaw   = angles.y * 0.5f;
	const float roll  = angles.r * 0.5f;

	const float sp = math::sin( pitch );
	const float cp = math::cos( pitch );

	const float sy = math::sin( yaw );
	const float cy = math::cos( yaw );

	const float sr = math::sin( roll );
	const float cr = math::cos( roll );

	x =  cr * sp * sy  -  sr * cp * cy;
	y = -cr * sp * cy  -  sr * cp * sy;
	z =  cr * cp * sy  -  sr * sp * cy;
	w =  cr * cp * cy  +  sr * sp * sy;
}

Quaternion::Quaternion( const DegAngles& angles ) : Quaternion( RadAngles( angles ) ) {}

void Quaternion::conjugate()
{
	x *= -1.0f;
	y *= -1.0f;
	z *= -1.0f;
}

void Quaternion::normalize()
{
	const float len = length();
	x /= len;
	y /= len;
	z /= len;
	w /= len;
}

float Quaternion::get_radian_pitch() const
{
	return 2.0f * math::atan2(
		math::sqrt( 1.0f + 2.0f * ( w * y - x * z ) ),
		math::sqrt( 1.0f - 2.0f * ( w * y - x * z ) )
	) - math::HALF_PI;
}

float Quaternion::get_radian_yaw() const
{
	return math::atan2(
		2.0f * ( w * z + x * y ),
		1.0f - 2.0f * ( y * y + z * z )
	);
}

float Quaternion::get_radian_roll() const
{
	return math::atan2(
		2.0f * ( w * x + y * z ),
		1.0f - 2.0f * ( x * x + y * y )
	);
}

RadAngles Quaternion::to_radians() const
{
	return RadAngles 
	{
		get_radian_pitch(),
		get_radian_yaw(),
		get_radian_roll()
	};
}

DegAngles Quaternion::to_degrees() const
{
	return DegAngles( to_radians() );
}

float Quaternion::length() const
{
	return math::sqrt( length_sqr() );
}

float Quaternion::length_sqr() const
{
	return x * x + y * y + z * z + w * w;
}

Quaternion Quaternion::normalize( const Quaternion& quat )
{
	Quaternion new_quat = quat;
	new_quat.normalize();
	return new_quat;
}

Quaternion Quaternion::lerp( 
	const Quaternion& a,
	const Quaternion& b,
	const float t
)
{
	Quaternion new_quat {};
	new_quat.x = math::lerp( a.x, b.x, t );
	new_quat.y = math::lerp( a.y, b.y, t );
	new_quat.z = math::lerp( a.z, b.z, t );
	new_quat.w = math::lerp( a.w, b.w, t );
	new_quat.normalize();
	return new_quat;
}

Quaternion Quaternion::slerp( 
	const Quaternion& a,
	const Quaternion& b,
	const float t
)
{
	const float raw_cosom = dot( a, b );

	float cosom = -raw_cosom;
	if ( raw_cosom >= 0.0f )
	{
		cosom = raw_cosom;
	}

	float scale0, scale1;
	if ( cosom < 0.9999f )
	{
		const float omega = math::acos( cosom );
		const float invSin = 1.f / math::sin( omega );
		scale0 = math::sin( ( 1.f - t ) * omega ) * invSin;
		scale1 = math::sin( t * omega ) * invSin;
	}
	else
	{
		// Use linear interpolation if the quaternions
		// are collinear
		scale0 = 1.0f - t;
		scale1 = t;
	}

	if ( raw_cosom < 0.0f )
	{
		scale1 = -scale1;
	}

	Quaternion new_quat;
	new_quat.x = scale0 * a.x + scale1 * b.x;
	new_quat.y = scale0 * a.y + scale1 * b.y;
	new_quat.z = scale0 * a.z + scale1 * b.z;
	new_quat.w = scale0 * a.w + scale1 * b.w;
	new_quat.normalize();
	return new_quat;
}

float Quaternion::dot(
	const Quaternion& a,
	const Quaternion& b
)
{
	return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}

Quaternion Quaternion::concatenate(
	const Quaternion& q,
	const Quaternion& p
)
{
	Quaternion new_quat;

	// Vector component is:
	// ps * qv + qs * pv + pv x qv
	const Vec3 qv( q.x, q.y, q.z );
	const Vec3 pv( p.x, p.y, p.z );
	const Vec3 newVec = p.w * qv + q.w * pv + Vec3::cross( pv, qv );
	new_quat.x = newVec.x;
	new_quat.y = newVec.y;
	new_quat.z = newVec.z;

	// Scalar component is:
	// ps * qs - pv . qv
	new_quat.w = p.w * q.w - Vec3::dot( pv, qv );

	return new_quat;
}

/*
 *  I didn't think I would say that one day but...
 *  Thanks Unreal for your code!
 * 
 *  I spent so much time trying to fix a weird 90 degrees offset 
 *  bug with the old look_at function I had (basically pointing 
 *  targets with their up axis instead of their forward), which 
 *  I believe was fundamentally messed up with my coordinates
 *  system.
 * 
 *  Therefore, as Unreal was using the same coordinates system 
 *  than me, I just looked at their code and adapted it.
 *  (also fixing the Quaternion constructor w/ Vec3 angles)
 * 
 *  It has been 3 weeks wondering if the issue was caused by 
 *  the look_at function, my coordinates system or my model 
 *  importer (or all of them in the same time)
 * 
 *  Happy to finally code again :)
 */
Quaternion Quaternion::look_at( const Vec3& forward, const Vec3& up )
{
	const Vec3 dir = forward.normalized();

	return Quaternion(
		RadAngles
		{
			math::atan2(
				dir.z,
				math::sqrt( dir.x * dir.x + dir.y * dir.y )
			),
			math::atan2( dir.y, dir.x ),
			0.0f
		}
	);
}

Quaternion Quaternion::look_at( const Vec3& origin, const Vec3& target, const Vec3& up )
{
	return look_at( target - origin, up );
}