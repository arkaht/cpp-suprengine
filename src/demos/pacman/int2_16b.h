#pragma once
#include <cstdint>

#include <suprengine/vec2.h>
#include <suprengine/math.h>

using suprengine::Vec2;

//  my own 2 bytes version of Vec2 to properly emulate 
//  the integer overflow Pac-Man had for handling directions
// 
//  I must use uint16_t for Y setter instead of uint_8t 
//  or I can't emulate the overflows (since the type truncates)
//
//  e.g.: Pinky's target have a X offset of 4 on the left (instead of none)
//		  when Pac-Man faces up, because:
//		  - when scaling the Y direction by 4: 0xFF * 0x4 = 0x03FC (where 0x03 overflows on the X byte)
//		  - when adding the direction & position: Y byte overflows on X byte again
struct Int2_16b
{
	static const Int2_16b up, down, left, right;

	int16_t value { 0 };

	Int2_16b( uint16_t value ) : value( value ) {}
	Int2_16b( uint16_t x, uint16_t y ) : value( x << 8 | y ) {}

	int8_t get_x() const
	{
		return value >> 8;
	}
	int8_t get_y() const
	{
		return value;
	}

	void set_x( uint16_t x )
	{
		value |= x << 8;
	}
	void set_y( uint16_t y )
	{
		value |= y;
	}

	float get_angle() const
	{
		return atan2( get_y(), get_x() ) * suprengine::RAD2DEG;
	}

	Int2_16b& operator+=( const Int2_16b& i )
	{
		value += i.value;
		return *this;
	}

	Int2_16b operator+( const Int2_16b& i )
	{
		return Int2_16b( value + i.value );
	}

	Vec2 to_vec2() const
	{
		return Vec2 {
			static_cast<float>( get_x() ),
			static_cast<float>( get_y() )
		};
	}
};