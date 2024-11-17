#include "color.h"

using namespace suprengine;

const Color Color::black { 0, 0, 0, 255 },
			Color::white { 255, 255, 255, 255 };
const Color Color::red { 255, 0, 0, 255 },
			Color::green { 0, 255, 0, 255 },
			Color::blue { 0, 0, 255, 255 };
const Color Color::gray { 127, 127, 127, 255 };
const Color Color::transparent { 255, 255, 255, 0 };