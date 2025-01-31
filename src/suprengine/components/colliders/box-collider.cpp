#include "box-collider.h"

#include <suprengine/components/transform.h>

#include <suprengine/tools/vis-debug.h>

using namespace suprengine;

const Vec3 normals[6] = 
{
	Vec3 { -1.0f, 0.0f, 0.0f }, Vec3 { 1.0f, 0.0f, 0.0f },
	Vec3 { 0.0f, -1.0f, 0.0f }, Vec3 { 0.0f, 1.0f, 0.0f },
	Vec3 { 0.0f, 0.0f, -1.0f }, Vec3 { 0.0f, 0.0f, 1.0f },
};

BoxCollider::BoxCollider( const Box& shape ) 
	: shape( shape ) 
{}

bool BoxCollider::intersects( SharedPtr<Collider> other )
{
	return false; 
}

bool BoxCollider::raycast( const Ray& ray, RayHit* hit, const RayParams& params )
{
	//  Source: 
	//	https://subscription.packtpub.com/book/game+development/9781787123663/14/ch14lvl1sec135/raycast-bounding-box

	//  TODO: Handle rotation (https://answers.unity.com/questions/532297/rotate-a-vector-around-a-certain-point.html)
	const Vec3 min = transform->location + shape.min * transform->scale,
			   max = transform->location + shape.max * transform->scale;

	//	Compute intersection points
	float t[] = {
		( min.x - ray.origin.x ) / ray.direction.x,
		( max.x - ray.origin.x ) / ray.direction.x,
		( min.y - ray.origin.y ) / ray.direction.y,
		( max.y - ray.origin.y ) / ray.direction.y,
		( min.z - ray.origin.z ) / ray.direction.z,
		( max.z - ray.origin.z ) / ray.direction.z,
	};

	//	Compute two extrems of all intersection points
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

	//	Check for intersection
	if ( max_t < 0.0f || min_t > max_t )
		return false;

	//	Check origin in box
	float result_t = min_t;
	if ( min_t <= 0.0f )
	{
		if ( !params.can_hit_from_origin )
			return false;

		result_t = max_t;
	}

	//	Check for maximum distance
	if ( result_t >= ray.distance )
		return false;

	//	Populate hit result
	hit->collider = as<Collider>();
	hit->point = ray.origin + ray.direction * result_t;

	//	Provide normal
	for ( int i = 0; i < 6; i++ )
	{
		if ( result_t == t[i] )
		{
			hit->normal = normals[i];
			break;
		}
	}

	return true;
}

void BoxCollider::debug_render( RenderBatch* render_batch )
{
	VisDebug::add_box(
		transform->location,
		transform->rotation,
		shape * transform->scale * 0.5f,
		is_active ? Color::green : Color::gray,
		0.0f,
		DebugChannel::Collider
	);
}
