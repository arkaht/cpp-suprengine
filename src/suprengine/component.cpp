#include "component.h"
#include "entity.h"

using namespace suprengine;

Component::Component( Entity* owner, int priority_order )
	: owner( owner ), priority_order( priority_order )
{
	transform = owner->transform;
}