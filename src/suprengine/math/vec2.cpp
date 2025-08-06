#include "vec2.h"

#include <imgui.h>

#include <suprengine/math/math.h>
#include <suprengine/math/vec3.h>

#include <suprengine/utils/assert.h>

using namespace suprengine;

const Vec2 Vec2::zero { 0.0f, 0.0f };
const Vec2 Vec2::one { 1.0f, 1.0f };

const Vec2 Vec2::up { 0.0f, -1.0f };
const Vec2 Vec2::down { 0.0f, 1.0f };
const Vec2 Vec2::left { -1.0f, 0.0f };
const Vec2 Vec2::right { 1.0f, 0.0f };

const Vec2 Vec2::unit_x { 1.0f, 0.0f };
const Vec2 Vec2::unit_y { 0.0f, 1.0f };

const Vec2 Vec2::infinity { math::PLUS_INFINITY, math::PLUS_INFINITY };

Vec2::Vec2( const ImVec2& vec ) : x( vec.x ), y( vec.y ) {}

Vec2::Vec2( const Vec3& vec ) : x( vec.x ), y( vec.y ) {}

float Vec2::length() const
{
	return math::sqrt( length_sqr() );
}

float Vec2::length_sqr() const
{
	return x * x + y * y;
}

Vec2& Vec2::normalize()
{
	const float mag = length();
	if ( mag == 0.0f ) return *this;

	x /= mag, y /= mag;
	return *this;
}

Vec2 Vec2::normalized() const
{
	Vec2 value( x, y );
	value.normalize();
	return value;
}

void Vec2::set_axis( const Axis2D axis, const float value )
{
	switch ( axis )
	{
		case Axis2D::X:
			x = value;
			break;
		case Axis2D::Y:
			y = value;
			break;
		default:
			ASSERT( false );
	}
}

float Vec2::get_angle() const
{
	return static_cast<float>( math::atan2( y, x ) );
}

SDL_Point Vec2::to_sdl_point() const
{
	return SDL_Point { static_cast<int>( x ), static_cast<int>( y ) };
}

std::string Vec2::to_string() const
{
	return "x=" + std::to_string( x ) + ";y=" + std::to_string( y );
}

float Vec2::distance( const Vec2& from, const Vec2& to )
{
	return ( from - to ).length();
}

float Vec2::distance_sqr( const Vec2& from, const Vec2& to )
{
	return ( from - to ).length_sqr();
}

Vec2 Vec2::direction( const Vec2& from, const Vec2& to )
{
	return ( to - from ).normalized();
}

Vec2 Vec2::world_to_grid( const Vec2& value, float grid_size )
{
	return Vec2 {
		math::floor( value.x / grid_size ),
		math::floor( value.y / grid_size ),
	};
}

Vec2 Vec2::snap_to_grid( const Vec2& value, float grid_size )
{
	return Vec2
	{
		math::snap_to_grid( value.x, grid_size ),
		math::snap_to_grid( value.y, grid_size ),
	};
}

Vec2 Vec2::snap_to_grid( const Vec2& value, const Vec2& grid_size )
{
	return Vec2
	{
		math::snap_to_grid( value.x, grid_size.x ),
		math::snap_to_grid( value.y, grid_size.y ),
	};
}

Vec2 Vec2::clamp( const Vec2& value, const Vec2& min, const Vec2& max )
{
	return Vec2 {
		math::clamp( value.x, min.x, max.x ),
		math::clamp( value.y, min.y, max.y ),
	};
}

Vec2 Vec2::round( const Vec2& value )
{
	return Vec2 {
		math::round( value.x ),
		math::round( value.y ),
	};
}

float Vec2::dot( const Vec2& a, const Vec2& b )
{
	return a.x * b.x + a.y * b.y;
}

Vec2 Vec2::lerp( const Vec2& a, const Vec2& b, const float t )
{
	return Vec2 {
		math::lerp( a.x, b.x, t ),
		math::lerp( a.y, b.y, t ),
	};
}

/*
 * For both slerp and nlerp, the implementation is from:
 * https://keithmaggio.wordpress.com/2011/02/15/math-magician-lerp-slerp-and-nlerp/
 */
Vec2 Vec2::slerp( const Vec2& a, const Vec2& b, float t )
{
	float dot = Vec2::dot( a, b );

	//	Clamp to ensure the range of acos
	dot = math::clamp( dot, -1.0f, 1.0f );

	const float theta = math::acos( dot ) * t;
	const Vec2 relative = ( b - a * t ).normalized();

	return a * math::cos( theta ) + relative * math::sin( theta );
}

Vec2 Vec2::nlerp( const Vec2& a, const Vec2& b, float t )
{
	return lerp( a, b, t ).normalized();
}

Vec2 Vec2::approach( const Vec2& current, const Vec2& target, float delta )
{
	return Vec2 {
		math::approach( current.x, target.x, delta ),
		math::approach( current.y, target.y, delta )
	};
}

Vec2& Vec2::operator+=( const Vec2& rhs )
{
	x += rhs.x;
	y += rhs.y;
	return *this;
}

Vec2& Vec2::operator-=( const Vec2& rhs )
{
	x -= rhs.x;
	y -= rhs.y;
	return *this;
}

Vec2& Vec2::operator*=( const Vec2& rhs )
{
	x *= rhs.x;
	y *= rhs.y;
	return *this;
}

Vec2& Vec2::operator*=( const float rhs )
{
	x *= rhs;
	y *= rhs;
	return *this;
}

Vec2& Vec2::operator/=( const Vec2& rhs )
{
	x /= rhs.x;
	y /= rhs.y;
	return *this;
}

Vec2& Vec2::operator/=( const float rhs )
{
	x /= rhs;
	y /= rhs;
	return *this;
}

Vec2 Vec2::operator-() const
{
	return Vec2 { -x, -y };
}

Vec2 suprengine::operator+( const Vec2& lhs, const Vec2& rhs )
{
	return Vec2( lhs.x + rhs.x, lhs.y + rhs.y );
}

Vec2 suprengine::operator-( const Vec2& lhs, const Vec2& rhs )
{
	return Vec2( lhs.x - rhs.x, lhs.y - rhs.y );
}

Vec2 suprengine::operator*( const Vec2& lhs, const Vec2& rhs )
{
	return Vec2( lhs.x * rhs.x, lhs.y * rhs.y );
}

Vec2 suprengine::operator*( const Vec2& lhs, const float rhs )
{
	return Vec2( lhs.x * rhs, lhs.y * rhs );
}

Vec2 suprengine::operator*( float lhs, const Vec2& rhs )
{
	return Vec2( lhs * rhs.x, lhs * rhs.y );
}

Vec2 suprengine::operator/( const Vec2& lhs, const Vec2& rhs )
{
	return Vec2( lhs.x / rhs.x, lhs.y / rhs.y );
}

Vec2 suprengine::operator/( const Vec2& lhs, const float rhs )
{
	return Vec2( lhs.x / rhs, lhs.y / rhs );
}

bool suprengine::operator==( const Vec2& lhs, const Vec2& rhs )
{
	return lhs.x == rhs.x && lhs.y == rhs.y;
}

bool suprengine::operator!=( const Vec2& lhs, const Vec2& rhs )
{
	return !( lhs == rhs );
}
