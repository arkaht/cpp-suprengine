#include "angles.h"

#include "math.h"

using namespace suprengine;

//
//  struct RadAngles
//

RadAngles::RadAngles()
{}

RadAngles::RadAngles( float a )
	: p( a ), y( a ), r( a )
{}

RadAngles::RadAngles( float p, float y, float r )
	: p( p ), y( y ), r( r )
{}

RadAngles::RadAngles( const Vec3& vec )
	: p( vec.x ), y( vec.y ), r( vec.z )
{}

RadAngles::RadAngles( const DegAngles& ang )
	: p( ang.p * math::DEG2RAD ), 
	  y( ang.y * math::DEG2RAD ), 
	  r( ang.r * math::DEG2RAD )
{}

float RadAngles::get_deg_pitch() const
{
	return p * math::RAD2DEG;
}

float RadAngles::get_deg_yaw() const
{
	return y * math::RAD2DEG;
}

float RadAngles::get_deg_roll() const
{
	return r * math::RAD2DEG;
}

//
//  struct DegAngles
//

DegAngles::DegAngles()
{}

DegAngles::DegAngles( float a )
	: p( a ), y( a ), r( a )
{}

DegAngles::DegAngles( float p, float y, float r )
	: p( p ), y( y ), r( r )
{}

DegAngles::DegAngles( const Vec3& vec )
	: p( vec.x ), y( vec.y ), r( vec.z )
{}

DegAngles::DegAngles( const RadAngles& ang )
	: p( ang.p * math::RAD2DEG ),
	  y( ang.y * math::RAD2DEG ),
	  r( ang.r * math::RAD2DEG )
{}

float DegAngles::get_radian_pitch() const
{
	return p * math::DEG2RAD;
}

float DegAngles::get_radian_yaw() const
{
	return y * math::DEG2RAD;
}

float DegAngles::get_radian_roll() const
{
	return r * math::DEG2RAD;
}
