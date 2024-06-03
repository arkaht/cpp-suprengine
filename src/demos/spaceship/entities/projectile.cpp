#include "projectile.h"

#include <entities/asteroid.h>
#include <entities/spaceship.h>

using namespace puzzle;

Projectile::Projectile( Spaceship* owner, Color color )
	: _owner( owner ), _color( color )
{
	_model_renderer = create_component<StylizedModelRenderer>(
		Assets::get_model( "projectile" ),
		color
	);
	_model_renderer->draw_only_outline = true;

	_lifetime_component = create_component<LifetimeComponent>( LIFETIME );
	_lifetime_component->on_time_out.listen( "owner", 
		std::bind( &Projectile::kill, this ) );
}

void Projectile::update_this( float dt )
{
	float movement_speed = move_speed * dt;
	Vec3 movement = transform->get_forward() * movement_speed;
	Vec3 new_location = transform->location + movement;
	if ( _check_collisions( movement_speed ) ) return;

	//  movement
	transform->set_location( new_location );
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

	//  damage health component
	if ( auto health = entity->get_component<HealthComponent>() )
	{
		DamageInfo info {};
		info.attacker = _owner;
		info.damage = damage_amount;
		info.knockback = -result.normal * knockback_force;

		DamageResult result = health->damage( info );

		//  alert owner
		if ( result.is_valid && _owner )
		{
			_owner->on_hit.invoke( result );
		}
	}
}
