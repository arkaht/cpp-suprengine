#include "entity.h"

using namespace suprengine;

int Entity::_global_id { 0 };

Entity::Entity()
	: _engine( &Engine::instance() )
{
	_unique_id = _global_id++;
}

Entity::~Entity()
{
	//  release components
	while ( !components.empty() )
	{
		auto& component = components.back();
		remove_component( component );
	}
}

void Entity::add_component( shared_ptr<Component> component )
{
	if ( std::find( components.begin(), components.end(), component ) != components.end() ) return;

	//  get update order
	int order = component->get_priority_order();

	//  search order position
	auto itr = components.begin();
	for ( ; itr != components.end(); itr++ )
	{
		if ( order >= (*itr)->get_priority_order() )
		{
			break;
		}
	}

	//  insert it
	components.insert( itr, component );

	component->setup();
}

void Entity::remove_component( shared_ptr<Component> component )
{
	auto itr = std::find( components.begin(), components.end(), component );
	if ( itr == components.end() ) return;

	component->unsetup();

	//  erase component
	components.erase( itr );
}

void Entity::init()
{
	//  assign a transform
	transform = create_component<Transform>();
}

void Entity::update( float dt )
{
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
