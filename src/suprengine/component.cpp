#include "component.h"
#include "entity.h"

using namespace suprengine;

Component::Component()
	: _priority_order( 0 )
{}

Component::Component( int priority_order )
	: _priority_order( priority_order )
{}

void Component::init( SharedPtr<Entity> owner )
{
	_owner = owner;
	transform = owner->transform;
}

SharedPtr<Entity> Component::get_owner() const 
{
	return _owner.lock();
}

int Component::get_priority_order() const 
{ 
	return _priority_order; 
}
