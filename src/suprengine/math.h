#pragma once

#include <limits>
#include <string>

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
	int   pow( int value, int exp );
	float sqrt( float value );
	float fmod( float value, float div );
	float modulo( float value, float div );

	float floor( float value );
	float ceil( float value );
	float round( float value );

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
	std::string to_hex( T value, size_t hex_length = sizeof( T ) * 2 )
	{
		static const char* DIGITS = "0123456789ABCDEF";
		std::string hex( hex_length, '0' );

		int i = (int)hex_length - 1;
		uint8_t quotient = value;
		do
		{
			uint8_t next_quotient = quotient / 16;
			uint8_t remainder = quotient % 16;
			hex[i--] = DIGITS[remainder];
			quotient = next_quotient;
		}
		while( quotient != 0 );

		return hex;
	}
	uint8_t from_hex( std::string hex );

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