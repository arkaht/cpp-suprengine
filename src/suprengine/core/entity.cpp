#include "entity.h"

using namespace suprengine;

int Entity::_global_id { 0 };

Entity::Entity()
{
	_unique_id = _global_id++;
}

Entity::~Entity()
{
	//  Release components
	while ( !components.empty() )
	{
		auto& component = components.back();
		remove_component( component );
	}
}

void Entity::add_component( SharedPtr<Component> component )
{
	if ( std::find( components.begin(), components.end(), component ) != components.end() ) return;

	//  Get update order
	int order = component->get_priority_order();

	//  Search insertion iterator
	auto itr = components.begin();
	for ( ; itr != components.end(); itr++ )
	{
		if ( order >= (*itr)->get_priority_order() )
		{
			break;
		}
	}

	//  Insert into vector
	components.insert( itr, component );

	component->setup();
}

void Entity::remove_component( SharedPtr<Component> component )
{
	auto itr = std::find( components.begin(), components.end(), component );
	if ( itr == components.end() ) return;

	component->unsetup();

	//  Erase component from vector
	components.erase( itr );
}

void Entity::init()
{
	//  Create and assign a transform
	transform = create_component<Transform>();
}

void Entity::update( float dt )
{
	//  Update components first
	for ( auto& component : components )
	{
		if ( !component->is_active ) continue;
		component->update( dt );
	}

	update_this( dt );
}

void Entity::kill()
{
	state = EntityState::Invalid;
}
