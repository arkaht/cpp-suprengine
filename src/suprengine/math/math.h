#pragma once

#include <limits>
#include <string>
#include <cmath>

namespace suprengine::math
{
	inline constexpr float PI = 3.141592f;
	inline constexpr float DOUBLE_PI = PI * 2.0f;
	inline constexpr float HALF_PI = PI / 2.0f;
	inline constexpr float DEG2RAD = PI / 180.0f;
	inline constexpr float RAD2DEG = 180.0f / PI;

	inline constexpr float PLUS_INFINITY = std::numeric_limits<float>::infinity();
	inline constexpr float NEG_INFINITY = -std::numeric_limits<float>::infinity();

	inline constexpr float EPSILON = 0.001f;

	inline float abs( float value )
	{
		return fabs( value );
	}
	inline float sign( float value )
	{
		return value >= 0.0f ? 1.0f : -1.0f;
	}
	
	inline float pow( float value, float exp )
	{
		return powf( value, exp );
	}
	inline int   pow( int value, int exp )
	{
		return (int)std::pow( value, exp );
	}
	inline float sqrt( float value )
	{
		return sqrtf( value );
	}
	inline float fmod( float value, float div )
	{
		return std::fmod( value, div );
	}
	inline float modulo( float value, float div )
	{
		if ( value >= 0 ) return fmod( value, div );
		return value - floor( value / div ) * div;
	}

	inline float floor( float value )
	{
		return std::floor( value );
	}
	inline float ceil( float value )
	{
		return std::ceil( value );
	}
	inline float round( float value )
	{
		return std::round( value );
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
	
	inline float approach( float current, float target, float delta )
	{
		delta = abs( delta );

		if ( current < target )
		{
			return fmin( current + delta, target );
		}
		else if ( current > target )
		{
			return fmax( current - delta, target );
		}

		return target;
	}
	inline float snap_to_grid( float value, float grid_size )
	{
		return floor( value / grid_size ) * grid_size;
	}

	inline bool near_zero( float val, float epsilon = EPSILON )
	{
		return abs( val ) <= epsilon;
	}
	inline bool near_value( float value, float target, float epsilon = EPSILON )
	{
		float diff = abs( value - target );
		return diff <= epsilon;
	}

	inline float normalize_angle( float ang )
	{
		return modulo( ang + PI, DOUBLE_PI ) - PI;
	}
	inline float remap(
		float value,
		float low1, float high1,
		float low2, float high2
	)
	{
		return low2 + ( value - low1 ) * ( high2 - low2 ) / ( high1 - low1 );
	}

	template <typename T>
	inline std::string to_hex( T value, size_t hex_length = sizeof( T ) * 2 )
	{
		constexpr const char* DIGITS = "0123456789ABCDEF";
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
	inline uint8_t from_hex( std::string hex )
	{
		//  Remove optional hexadecimal indicator
		if ( hex[0] == '#' )
		{
			hex = hex.erase( 0, 1 );
		}

		return std::stoi( hex, nullptr, 16 );
	}

	template <typename T>
	inline T max( const T& a, const T& b )
	{
		return a < b ? b : a;
	}

	template <typename T>
	inline T min( const T& a, const T& b )
	{
		return a < b ? a : b;
	}

	template <typename T>
	inline T clamp( const T& value, const T& lower, const T& upper )
	{
		return min( upper, max( lower, value ) );
	}
	
	template <typename T>
	inline T lerp( T a, T b, float t )
	{
		return a + static_cast<T>( t * ( b - a ) );
	}
}