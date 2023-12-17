#pragma once

#include "math.h"

//  https://easings.net/
namespace suprengine::easing
{
	template <typename T>
	static T in_sine( T x ) { return 1.0f - math::cos( ( x * math::PI ) * 0.5f ); }

	template <typename T>
	static T in_cubic( T x ) { return x * x * x; }
}