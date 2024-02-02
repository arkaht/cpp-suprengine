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
}

void GuidedMissile::update_this( float dt )
{
	if ( auto target = _wk_target.lock() )
	{
		Vec3 diff = target->location - transform->location;

		float distance = diff.length();
		if ( distance <= impact_distance )
		{
			auto health = target->get_owner()->get_component<HealthComponent>();
			if ( health )
			{
				DamageInfo info {};
				info.attacker = _owner;
				info.damage = damage_amount;

				DamageResult result = health->damage( info );

				//  alert owner
				if ( result.is_valid && _owner )
				{
					_owner->on_hit.invoke( result );
				}
			}
			
			printf( "boom\n" );
			kill();
			return;
		}

		Vec3 direction = diff * ( 1.0f / distance );
		Quaternion look_rotation = Quaternion::look_at( direction, up_direction );
		transform->set_rotation( 
			Quaternion::slerp( 
				transform->rotation,
				look_rotation,
				dt * rotation_speed
			)
		);
	}

	transform->set_location( 
		transform->location 
	  + transform->get_forward() * move_speed * dt 
	);
}
