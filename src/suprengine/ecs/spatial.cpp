#include "spatial.h"

#include "components/transform3.hpp"

using namespace suprengine;

Spatial::Spatial()
	: Entity()
{
	transform = new Transform3( this );
}
