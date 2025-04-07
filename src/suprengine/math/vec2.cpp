#include "vec2.h"

using namespace suprengine;

const Vec2 Vec2::zero { 0.0f, 0.0f }, 
		   Vec2::one { 1.0f, 1.0f },
		   Vec2::up { 0.0f, -1.0f },
		   Vec2::down { 0.0f, 1.0f },
		   Vec2::left { -1.0f, 0.0f },
	       Vec2::right { 1.0f, 0.0f };

float Vec2::dot( const Vec2& a, const Vec2& b )
{
	return a.x * b.x + a.y * b.y;
}
