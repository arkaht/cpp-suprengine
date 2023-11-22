#include "entity.h"

using namespace suprengine;

int Entity::_global_id { 0 };

Entity::Entity()
	: _game( &Game::instance() )
{
	_unique_id = _global_id++;

	//  assign a transform
	transform = create_component<Transform>();

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
		auto& component = components.back();
		component->unsetup();

		remove_component( component );
	}
}

void Entity::add_component( std::shared_ptr<Component> component )
{
	if ( std::find( components.begin(), components.end(), component ) != components.end() ) return;

	//  get update order
	int order = component->get_priority_order();

	//  search order position
	auto itr = components.begin();
	for ( ; itr != components.end(); itr++ )
		if ( order >= (*itr)->get_priority_order() )
			break;

	//  insert it
	components.insert( itr, component );
}

void Entity::remove_component( std::shared_ptr<Component> component )
{
	auto itr = std::find( components.begin(), components.end(), component );
	if ( itr == components.end() ) return;

	//  erase component
	components.erase( itr );
}

void Entity::update( float dt )
{
	for ( auto& component : components )
	{
		//  init
		if ( !component->is_initialized )
		{
			component->init();
			component->is_initialized = true;
		}

		//  update
		if ( !component->should_update ) continue;
		component->update( dt );
	}

	update_this( dt );
}

void Entity::kill()
{
	state = EntityState::DEAD;
}
