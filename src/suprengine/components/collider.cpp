#include "collider.h"
#include "../entity.h"

using namespace suprengine;

Collider::Collider( int priority_order )
	: Component( priority_order )
{}

void Collider::setup()
{
	auto physics = Engine::instance().get_physics();

	auto ptr = as<Collider>();
	physics->add_collider( ptr );
}

void Collider::unsetup()
{
	auto physics = Engine::instance().get_physics();

	auto ptr = as<Collider>();
	physics->remove_collider( ptr );
}

void Collider::update_collision_with( shared_ptr<Collider> other, bool is_active )
{
	Collider* other_ptr = other.get();
	bool is_found = collisions.find( other_ptr ) != collisions.end();

	if ( is_active )
	{
		if ( !is_found )
		{
			collisions.emplace( other_ptr );
			on_collision_update.invoke( other, CollisionState::Enter );
		}
		else
		{
			on_collision_update.invoke( other, CollisionState::Stay );
		}
	}
	else
	{
		if ( is_found )
		{
			collisions.erase( other_ptr );
			on_collision_update.invoke( other, CollisionState::Exit );
		}
	}
}
