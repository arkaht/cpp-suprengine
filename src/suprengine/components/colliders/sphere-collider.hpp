#pragma once
#include "collider.h"

#include "transform.h"

namespace suprengine
{
	class SphereCollider : public Collider
	{
	public:
		float radius { 0.0f };

		SphereCollider( Entity* owner, float radius ) : radius( radius ), Collider( owner ) {};

		bool intersects( Collider* other ) override { return false; }  //  TODO: implement this

		//  source: https://gdbooks.gitbooks.io/3dcollisions/content/Chapter3/raycast_sphere.html
		bool raycast( _RAYCAST_FUNC_PARAMS )
		{
			Vec3 center = transform->location;
			float _radius = radius * transform->scale.x;

			Vec3 e = center - ray.origin;
			float a = Vec3::dot( e, ray.direction );
			
			//  check intersection
			float esq = e.length_sqr();
			float radius_sqr = _radius * _radius;
			float f_sqr = radius_sqr - esq + a * a;
			if ( f_sqr < 0.0f )
				return false;

			float f = math::sqrt( f_sqr );
			float result_t = a - f;

			//  check origin in sphere
			if ( esq < radius_sqr )
			{
				if ( !params.can_hit_from_origin )
					return false;

				result_t = a + f;
			}

			if ( result_t < 0.0f ) return false;

			//  check max distance
			if ( result_t >= ray.distance ) 
				return false;

			//  hit result
			hit->collider = this;
			hit->point = ray.origin + ray.direction * result_t;
			hit->normal = ( hit->point - center ).normalized();

			return true;
		}

		void debug_render( RenderBatch* render_batch ) override  //  TODO: implement this
		{
			//Assets::get_mesh( Assets::PRIMITIVE_CUBE_PATH, false );
			//render_batch->draw_mesh(  )
		}
	};
}