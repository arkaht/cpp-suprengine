#pragma once

#include "math.h"

//  https://easings.net/
namespace suprengine::easing
{
	template <typename T>
	static T in_sine( T x ) { return 1.0f - math::cos( ( x * math::PI ) * 0.5f ); }

	template <typename T>
	static T in_cubic( T x ) { return x * x * x; }
	template <typename T>
	static T in_out_cubic( T x ) 
	{ 
		return x < 0.5f 
			 ? 4.0f * x * x * x
			 : 1.0f - math::pow( -2.0f * x + 2.0f, 3.0f ) * 0.5f;
	}
}