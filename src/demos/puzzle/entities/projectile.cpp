#include "projectile.h"

#include <entities/asteroid.h>
#include <entities/spaceship.h>

using namespace puzzle;

Projectile::Projectile( Spaceship* owner, Color color )
	: _owner( owner ), _color( color )
{
	model_renderer = create_component<StylizedModelRenderer>(
		Assets::get_model( "projectile" ),
		color
	);
	model_renderer->draw_only_outline = true;
}

void Projectile::update_this( float dt )
{
	float movement_speed = move_speed * dt;
	Vec3 movement = transform->get_forward() * movement_speed;
	Vec3 new_location = transform->location + movement;
	if ( _check_collisions( movement_speed ) ) return;

	//  movement
	transform->set_location( new_location );

	//  life time
	life_time -= dt;
	if ( life_time <= 0.0f )
	{
		kill();
	}
}

bool Projectile::_check_collisions( float movement_speed )
{
	auto physics = _engine->get_physics();

	//  setup ray
	Ray ray( 
		transform->location, 
		transform->get_forward(), 
		movement_speed 
	);
	RayParams params {};

	//  check safe movement 
	RayHit result;
	if ( physics->raycast( ray, &result, params ) )
	{
		if ( result.collider->get_owner() != _owner )
		{
			_on_hit( result );
			kill();
			return true;
		}
	}

	return false;
}

void Projectile::_on_hit( const RayHit& result )
{
	auto entity = result.collider->get_owner();

	//  TODO: Create a Health component 
	//  damage asteroid
	if ( auto asteroid = dynamic_cast<Asteroid*>( entity ) )
	{
		asteroid->damage( 
			damage_amount, 
			-result.normal * knockback_force 
		);
		printf( "hit asteroid %d!\n", asteroid->get_unique_id() );
	}

	//  alert owner
	if ( _owner )
	{
		_owner->on_hit.invoke( entity );
	}
}
