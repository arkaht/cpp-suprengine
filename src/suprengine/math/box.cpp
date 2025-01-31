#include "box.h"

using namespace suprengine;

Box Box::one {
	Vec3 { -1.0f, -1.0f, -1.0f },
	Vec3 { 1.0f, 1.0f, 1.0f }
};

Box Box::half {
	Vec3 { -0.5f, -0.5f, -0.5f },
	Vec3 { 0.5f, 0.5f, 0.5f }
};


Vec3 Box::get_center() const
{
	return ( min + max ) * 0.5f;
}

Vec3 Box::get_size() const
{
	return max - min;
}
