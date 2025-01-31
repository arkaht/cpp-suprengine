#include "sphere-collider.h"

#include <suprengine/components/transform.h>

#include <suprengine/tools/vis-debug.h>

using namespace suprengine;

SphereCollider::SphereCollider( float radius )
	: radius( radius ) 
{}

bool SphereCollider::intersects( SharedPtr<Collider> other )
{ 
	return false; 
}

bool SphereCollider::raycast( const Ray& ray, RayHit* hit, const RayParams& params )
{
	//	Source: https://gdbooks.gitbooks.io/3dcollisions/content/Chapter3/raycast_sphere.html

	const Vec3 center = transform->location;
	const float scaled_radius = get_scaled_radius();

	const Vec3 offset = center - ray.origin;
	const float a = Vec3::dot( offset, ray.direction );

	//  Check intersection
	const float esq = offset.length_sqr();
	const float squared_radius = scaled_radius * scaled_radius;
	const float f_sqr = squared_radius - esq + a * a;
	if ( f_sqr < 0.0f ) return false;

	const float f = math::sqrt( f_sqr );
	float result_t = a - f;

	//	Check origin in sphere
	if ( esq < squared_radius )
	{
		if ( !params.can_hit_from_origin ) return false;

		result_t = a + f;
	}

	if ( result_t < 0.0f ) return false;

	//	Check max distance
	if ( result_t >= ray.distance ) return false;

	//	Populate hit result
	hit->collider = as<Collider>();
	hit->point = ray.origin + ray.direction * result_t;
	hit->normal = ( hit->point - center ).normalized();

	return true;
}

float SphereCollider::get_scaled_radius() const
{
	return radius * transform->scale.x;
}

void SphereCollider::debug_render( RenderBatch* render_batch )
{
	VisDebug::add_sphere(
		transform->location,
		get_scaled_radius(),
		is_active ? Color::green : Color::gray,
		0.0f,
		DebugChannel::Collider
	);
}
