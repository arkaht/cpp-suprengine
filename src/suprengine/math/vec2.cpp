#include "vec2.h"

#include "suprengine/utils/assert.h"

using namespace suprengine;

const Vec2 Vec2::zero { 0.0f, 0.0f }, Vec2::one { 1.0f, 1.0f }, Vec2::up { 0.0f, -1.0f },
	Vec2::down { 0.0f, 1.0f }, Vec2::left { -1.0f, 0.0f }, Vec2::right { 1.0f, 0.0f };

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

float Vec2::dot( const Vec2& a, const Vec2& b )
{
	return a.x * b.x + a.y * b.y;
}

std::string Vec2::to_string() const
{
	return "x=" + std::to_string( x )
		+ ";y=" + std::to_string( y );
}
