#include "sphere-collider.h"

using namespace suprengine;

SphereCollider::SphereCollider( float radius )
	: radius( radius ) 
{}

bool SphereCollider::intersects( shared_ptr<Collider> other )
{ 
	return false; 
}

bool SphereCollider::raycast( _RAYCAST_FUNC_PARAMS )
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
	hit->collider = as<Collider>();
	hit->point = ray.origin + ray.direction * result_t;
	hit->normal = ( hit->point - center ).normalized();

	return true;
}

void SphereCollider::debug_render( RenderBatch* render_batch )
{
	auto model = Assets::get_model( Assets::SPHERE_PATH );
	Mtx4 matrix = Mtx4::create_from_transform( 
		radius * transform->scale, 
		Quaternion::identity,
		transform->location 
	);

	render_batch->draw_debug_model(
		matrix,
		model,
		is_active ? Color::green : Color::red
	);
}
