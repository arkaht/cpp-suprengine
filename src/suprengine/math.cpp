#include "math.h"

#include <cmath>

using namespace suprengine;

float math::abs( float value )
{
	return fabs( value );
}

float math::sign( float value )
{
	return value >= 0.0f ? 1.0f : -1.0f;
}

float math::normalize_angle( float ang )
{
	return modulo( ang + PI, DOUBLE_PI ) - PI;
}

float math::remap( float value, float low1, float high1, float low2, float high2 )
{
	return low2 + ( value - low1 ) * ( high2 - low2 ) / ( high1 - low1 );
}

uint8_t math::from_hex( std::string hex )
{
	//  Remove optional hexadecimal indicator
	if ( hex[0] == '#' )
	{
		hex = hex.erase( 0, 1 );
	}

	return std::stoi( hex, nullptr, 16 );
}

float math::pow( float value, float exp )
{
	return powf( value, exp );
}

int math::pow( int value, int exp )
{
	return (int)std::pow( value, exp );
}

float math::sqrt( float value )
{
	return sqrtf( value );
}

float math::fmod( float value, float div )
{
	return std::fmod( value, div );
}

float math::modulo( float value, float div )
{
	if ( value >= 0 )
		return fmod( value, div );

	return value - floor( value / div ) * div;
}

float math::floor( float value )
{
	return std::floor( value );
}

float math::ceil( float value )
{
	return std::ceil( value );
}

float math::round( float num )
{
	return std::round( num );
}

float math::cos( float angle )
{
	return cosf( angle );
}

float math::sin( float angle )
{
	return sinf( angle );
}

float math::tan( float angle )
{
	return tanf( angle );
}

float math::acos( float value )
{
	return acosf( value );
}

float math::atan2( float y, float x )
{
	return atan2f( y, x );
}

float math::cot( float angle )
{
	return 1.0f / tan( angle );
}

float math::approach( float current, float target, float delta )
{
	delta = abs( delta );

	if ( current < target )
		return fmin( current + delta, target );
	else if ( current > target )
		return fmax( current - delta, target );

	return target;
}

float math::snap_to_grid( float value, float grid_size )
{
	return floor( value / grid_size ) * grid_size;
}

bool math::near_zero( float val, float epsilon )
{
	return abs( val ) <= epsilon;
}

bool math::near_value( float value, float target, float epsilon )
{
	float diff = abs( value - target );
	return diff <= epsilon;
}