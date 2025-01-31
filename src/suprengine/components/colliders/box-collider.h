#pragma once
#include <suprengine/components/collider.h>

#include <suprengine/math/box.h>

namespace suprengine
{
	class BoxCollider : public Collider
	{
	public:
		Box shape;

		BoxCollider( const Box& shape );

		//  TODO: implement this
		bool intersects( SharedPtr<Collider> other ) override;
		bool raycast( const Ray& ray, RayHit* hit, const RayParams& params );

		void debug_render( RenderBatch* render_batch ) override;
	};
}