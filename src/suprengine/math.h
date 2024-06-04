#pragma once

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

	float abs( float value );
	float sign( float value );
	
	float pow( float value, float exp );
	float sqrt( float value );
	float fmod( float value, float div );
	float modulo( float value, float div );
	int   round( float num );

	float cos( float angle );
	float sin( float angle );
	float tan( float angle );

	float acos( float value );
	float atan2( float y, float x );
	float cot( float angle );
	
	float approach( float current, float target, float delta );
	float snap_to_grid( float value, float grid_size );

	bool near_zero( float val, float epsilon = EPSILON );
	bool near_value( float value, float target, float epsilon = EPSILON );

	float normalize_angle( float ang );
	float remap( 
		float value, 
		float low1, float high1, 
		float low2, float high2 
	);

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
	
	template <typename T>
	T lerp( T a, T b, float f )
	{
		return a + (T)( f * ( b - a ) );
	}
}