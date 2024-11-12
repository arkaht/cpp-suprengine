#include "box.h"

using namespace suprengine;

Box Box::ONE {
	Vec3 { -1.0f, -1.0f, -1.0f },
	Vec3 { 1.0f, 1.0f, 1.0f }
};

Box Box::HALF {
	Vec3 { -0.5f, -0.5f, -0.5f },
	Vec3 { 0.5f, 0.5f, 0.5f }
};