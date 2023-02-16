#include "entity2d.h"

#include "components/transform2.hpp"

using namespace suprengine;

Entity2D::Entity2D() : Entity()
{
	//  assign a transform
	transform = new Transform2( this );
}