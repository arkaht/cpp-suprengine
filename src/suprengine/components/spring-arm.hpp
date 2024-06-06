#pragma once
#include <suprengine/component.h>
#include <suprengine/entity.h>
#include <suprengine/physics.h>

namespace suprengine
{
	class SpringArm : public Component
	{
	private:
	public:
		shared_ptr<Transform> target;
		Vec3 spring_offset;
		Vec3 look_offset;
		float hit_normal_offset { 0.1f };

		SpringArm( 
			shared_ptr<Entity> owner, 
			shared_ptr<Transform> target, 
			const Vec3& spring_offset = Vec3 { 0.0f, 1.0f, 5.0f }, 
			const Vec3& look_offset = Vec3::zero 
		)
			: target( target ), spring_offset( spring_offset ), Component( owner ) {}

		void update( float dt ) override
		{
			auto physics = get_owner()->get_engine()->get_physics();

			//  get ideal new position
			Vec3 pos = target->location + Vec3::transform( spring_offset, target->rotation );

			//  setup ray structs
			Ray ray( target->location, pos - target->location );
			RayParams params;
			params.can_hit_from_origin = false;

			//  raycast to new position
			RayHit hit;
			if ( physics->raycast( ray, &hit, params ) )
			{
				pos = hit.point + hit.normal * hit_normal_offset;
				//printf( "hit at %f %f %f on %p\n", hit.point.x, hit.point.y, hit.point.z, hit.collider );
			}

			//  apply position & rotation
			transform->set_location( pos );
			transform->set_rotation( Quaternion::look_at( transform->location, target->location + look_offset, Vec3::up ) );
		}
	};
}