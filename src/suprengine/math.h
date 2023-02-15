#pragma once

#include <cmath>
#include <limits>

namespace suprengine::math
{
	constexpr double PI = 3.141592653589793;
	constexpr double DOUBLE_PI = PI * 2.0;
	constexpr double HALF_PI = PI / 2.0;
	constexpr double DEG2RAD = PI / 180.0;
	constexpr double RAD2DEG = 180.0 / PI;

	constexpr float PLUS_INFINITY = std::numeric_limits<float>::infinity();
	const float NEG_INFINITY = -std::numeric_limits<float>::infinity();

	static float approach( float current, float target, float delta )
	{
		delta = abs( delta );

		if ( current < target )
			return fmin( current + delta, target );
		else if ( current > target )
			return fmax( current - delta, target );

		return target;
	}

	inline bool near_zero( float val, float epsilon = 0.001f )
	{
		if ( fabs( val ) <= epsilon )
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	template <typename T>
	T max( const T& a, const T& b )
	{
		return ( a < b ? b : a );
	}

	template <typename T>
	T min( const T& a, const T& b )
	{
		return ( a < b ? a : b );
	}

	template <typename T>
	T clamp( const T& value, const T& lower, const T& upper )
	{
		return min( upper, max( lower, value ) );
	}

	inline float abs( float value )
	{
		return fabs( value );
	}

	inline float cos( float angle )
	{
		return cosf( angle );
	}

	inline float sin( float angle )
	{
		return sinf( angle );
	}

	inline float tan( float angle )
	{
		return tanf( angle );
	}

	inline float acos( float value )
	{
		return acosf( value );
	}

	inline float atan2( float y, float x )
	{
		return atan2f( y, x );
	}

	inline float cot( float angle )
	{
		return 1.0f / tan( angle );
	}

	inline float lerp( float a, float b, float f )
	{
		return a + f * ( b - a );
	}

	inline float sqrt( float value )
	{
		return sqrtf( value );
	}

	inline float fmod( float numer, float denom )
	{
		return std::fmod( numer, denom );
	}

	inline int round( float num )
	{
		return static_cast<int>( std::round( num ) );
	}
}