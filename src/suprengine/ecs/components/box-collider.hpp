#pragma once
#include "collider.h"

#include "transform.h"

#include <suprengine/box.hpp>

namespace suprengine
{
	class BoxCollider : public Collider
	{
	private:
		const Vec3 normals[6] = {
			Vec3 { -1.0f, 0.0f, 0.0f }, Vec3 { 1.0f, 0.0f, 0.0f },
			Vec3 { 0.0f, -1.0f, 0.0f }, Vec3 { 0.0f, 1.0f, 0.0f },
			Vec3 { 0.0f, 0.0f, -1.0f }, Vec3 { 0.0f, 0.0f, 1.0f },
		};

	public:
		Box shape;

		BoxCollider( Entity* owner, const Box& shape ) : shape( shape ), Collider( owner ) {};

		bool intersects( Collider* other ) override { return false; }  //  TODO: implement this

		//  source: https://subscription.packtpub.com/book/game+development/9781787123663/14/ch14lvl1sec135/raycast-bounding-box
		bool raycast( const Ray& ray, RayHit* hit )
		{
			//  get bounds
			Vec3 min = transform->location + shape.min * transform->scale,  //  TODO: handle rotation (https://answers.unity.com/questions/532297/rotate-a-vector-around-a-certain-point.html)
				 max = transform->location + shape.max * transform->scale;

			//  get intersection points
			float t[] = {
				( min.x - ray.origin.x ) / ray.direction.x,
				( max.x - ray.origin.x ) / ray.direction.x,
				( min.y - ray.origin.y ) / ray.direction.y,
				( max.y - ray.origin.y ) / ray.direction.y,
				( min.z - ray.origin.z ) / ray.direction.z,
				( max.z - ray.origin.z ) / ray.direction.z,
			};

			//  get minimum & maximum
			float min_t = math::max(
				math::max(
					math::min( t[0], t[1] ),
					math::min( t[2], t[3] )
				),
				math::min( t[4], t[5] )
			);
			float max_t = math::min(
				math::min(
					math::max( t[0], t[1] ),
					math::max( t[2], t[3] )
				),
				math::max( t[4], t[5] )
			);

			//  check intersection
			if ( max_t < 0 || min_t > max_t ) 
				return false;

			float result_t = min_t < 0.0f ? max_t : min_t;
			//  TODO: check max distance
			//if ( result_t >= ray.distance ) 
			//	return false;

			//  hit result
			hit->collider = this;
			hit->point = ray.origin + ray.direction * result_t;

			//  provide normal
			for ( int i = 0; i < 6; i++ )
				if ( result_t == t[i] )
				{
					hit->normal = normals[i];
					break;
				}

			return true;
		}

		void debug_render( RenderBatch* render_batch ) override  //  TODO: implement this
		{
			//Assets::get_mesh( Assets::PRIMITIVE_CUBE_PATH, false );
			//render_batch->draw_mesh(  )
		}
	};
}