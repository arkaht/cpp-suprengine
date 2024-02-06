#include "guided-missile.h"

#include <components/health-component.h>
#include <entities/spaceship.h>

using namespace puzzle;

GuidedMissile::GuidedMissile( 
	Spaceship* owner, 
	std::weak_ptr<Transform> wk_target,
	Color color 
)
	: _owner( owner ), _wk_target( wk_target ),
	  Entity()
{
	_model_renderer = create_component<StylizedModelRenderer>(
		Assets::get_model( "projectile" ),
		color
	);
	_model_renderer->draw_only_outline = true;

	_lifetime_component = create_component<LifetimeComponent>( LIFETIME );
	_lifetime_component->on_time_out.listen( "owner", 
		std::bind( &GuidedMissile::kill, this ) );
}

void GuidedMissile::update_this( float dt )
{
	//  accelerate speeds
	_current_move_speed = math::lerp( 
		_current_move_speed, 
		move_speed, 
		dt * move_acceleration 
	);
	_current_rotation_speed = math::lerp(
		_current_rotation_speed,
		rotation_speed,
		dt * rotation_acceleration
	);

	_update_target( dt );

	//  move forward
	transform->set_location( 
		transform->location 
	  + transform->get_forward() * _current_move_speed * dt
	);

	_check_impact();
}

void GuidedMissile::_update_target( float dt )
{
	auto target = _wk_target.lock();
	if ( !target ) return;

	Vec3 diff = target->location - transform->location;

	//  explode if close enough
	float distance = diff.length();
	/*if ( distance <= impact_distance )
	{
		auto health = target->get_owner()->get_component<HealthComponent>();
		_damage( health );
		return;
	}*/

	//  rotate towards target
	Vec3 direction = diff * ( 1.0f / distance );
	Quaternion look_rotation = Quaternion::look_at( direction, up_direction );
	transform->set_rotation(
		Quaternion::slerp(
			transform->rotation,
			look_rotation,
			dt * _current_rotation_speed
		)
	);
}

void GuidedMissile::_check_impact()
{
	auto physics = _engine->get_physics();

	//  setup raycast
	Ray ray( 
		transform->location, 
		transform->get_forward(), 
		impact_distance 
	);
	RayParams params {};
	params.can_hit_from_origin = false;

	//  check collisions
	RayHit hit {};
	if ( !physics->raycast( ray, &hit, params ) ) return;
	
	//  check entity has health component
	auto entity = hit.collider->get_owner();
	auto health = entity->get_component<HealthComponent>();
	if ( !health ) return;
	
	//  damage it
	_damage( health );
	//printf( "hit %p (self:%p)\n", hit.collider->get_owner(), this );
}

void GuidedMissile::_damage( std::shared_ptr<HealthComponent> target )
{
	if ( !target ) return;
	
	Vec3 diff = target->transform->location - transform->location;

	//  damage
	DamageInfo info {};
	info.attacker = _owner;
	info.damage = damage_amount;
	info.knockback = diff.normalized() * knockback_force;
	DamageResult result = target->damage( info );

	//  alert owner
	if ( result.is_valid && _owner )
	{
		_owner->on_hit.invoke( result );
	}

	kill();
	//printf( "boom\n" );
}
