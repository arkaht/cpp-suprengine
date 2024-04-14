#pragma once

#include <cmath>
#include <limits>

namespace suprengine::math
{
	constexpr float PI = 3.141592f;
	constexpr float DOUBLE_PI = PI * 2.0f;
	constexpr float HALF_PI = PI / 2.0f;
	constexpr float DEG2RAD = PI / 180.0f;
	constexpr float RAD2DEG = 180.0f / PI;

	constexpr float PLUS_INFINITY = std::numeric_limits<float>::infinity();
	constexpr float NEG_INFINITY = -std::numeric_limits<float>::infinity();

	constexpr float EPSILON = 0.001f;

	static float approach( float current, float target, float delta )
	{
		delta = abs( delta );

		if ( current < target )
			return fmin( current + delta, target );
		else if ( current > target )
			return fmax( current - delta, target );

		return target;
	}

	static bool near_zero( float val, float epsilon = EPSILON )
	{
		return fabs( val ) <= epsilon;
	}

	static bool near_value( float value, float target, float epsilon = EPSILON )
	{
		float diff = abs( value - target );
		return diff <= epsilon;
	}

	static float modulo( float value, float div )
	{
		if ( value >= 0 )
			return fmodf( value, div );

		return value - floor( value / div ) * div;
	}

	static float normalize_angle( float ang )
	{
		return modulo( ang + PI, DOUBLE_PI ) - PI;
	}

	static float sign( float value )
	{
		return value >= 0.0f ? 1.0f : -1.0f;
	}

	static float pow( float value, float exp )
	{
		return powf( value, exp );
	}

	static float remap( 
		float value, 
		float low1, float high1, 
		float low2, float high2 
	)
	{
		return low2 + ( value - low1 ) * ( high2 - low2 ) / ( high1 - low1 );
	}

	template <typename T>
	T max( const T& a, const T& b )
	{
		return a < b ? b : a;
	}

	template <typename T>
	T min( const T& a, const T& b )
	{
		return a < b ? a : b;
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

	template <typename T>
	inline T lerp( T a, T b, float f )
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