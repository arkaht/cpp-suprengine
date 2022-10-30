#pragma once

#include <cmath>

namespace suprengine::math
{
	constexpr double PI = 3.141592653589793;
	constexpr double DEG2RAD = PI / 180.0;
	constexpr double RAD2DEG = 180.0 / PI;

	static float approach( float current, float target, float delta )
	{
		delta = abs( delta );

		if ( current < target )
			return fmin( current + delta, target );
		else if ( current > target )
			return fmax( current - delta, target );

		return target;
	}
}