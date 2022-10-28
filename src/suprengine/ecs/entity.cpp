#include "entity.h"
#include "components/transform2.hpp"

using namespace suprengine;

Entity::Entity()
{
	//  assign a transform
	transform = new Transform2( this );

	//  register in game
	_game->add_entity( this );
}

Entity::~Entity()
{
	//  remove from game
	_game->remove_entity( this );

	//  release components
	while ( !components.empty() )
	{
		delete components.back();
	}
}

void Entity::add_component( Component* comp )
{
	if ( get_component_iterator( comp ) != components.end() ) return;

	//  get update order
	int order = comp->get_priority_order();

	//  search order position
	auto itr = components.begin();
	for ( ; itr != components.end(); itr++ )
	{
		if ( order < (*itr)->get_priority_order() )
		{
			break;
		}
	}

	//  insert it
	components.insert( itr, comp );
}

void Entity::remove_component( Component* comp )
{
	auto itr = get_component_iterator( comp );
	if ( itr == components.end() ) return;

	//  erase component
	components.erase( itr );
}

std::vector<Component*>::iterator Entity::get_component_iterator( Component* comp )
{
	return std::find( components.begin(), components.end(), comp );
}

void Entity::update( float dt )
{
	if ( _state != EntityState::ACTIVE ) return;

	for ( auto comp : components )
	{
		comp->update( dt );
	}
}

void Entity::kill()
{
	_state = EntityState::DEAD;
}
