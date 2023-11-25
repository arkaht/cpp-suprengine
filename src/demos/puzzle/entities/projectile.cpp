#include "projectile.h"
#include "asteroid.h"

using namespace puzzle;

Projectile::Projectile( Color color )
	: _color( color )
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

	//  check collisions
	//{
		//  setup ray
		Ray ray( transform->location, transform->get_forward(), movement_speed );
		RayHit result {};
		RayParams params {};

		//  check safe movement 
		auto physics = _game->get_physics();
		if ( physics->raycast( ray, &result, params ) )
		{
			on_hit( result );
			kill();
			return;
		}
	//}

	//  movement
	transform->set_location( new_location );

	//  life time
	life_time -= dt;
	if ( life_time <= 0.0f )
	{
		kill();
	}
}

void Projectile::on_hit( const RayHit& result )
{
	auto collider = result.collider.lock();
	if ( !collider ) return;

	auto entity = collider->get_owner();

	if ( auto asteroid = dynamic_cast<Asteroid*>( entity ) )
	{
		asteroid->damage( damage_amount, -result.normal * knockback_force );
		printf( "hit asteroid %d!\n", asteroid->get_unique_id() );
	}

}
