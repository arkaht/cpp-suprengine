#pragma once
#include <suprengine/components/collider.h>

#include <suprengine/components/transform.h>
#include <suprengine/box.h>

namespace suprengine
{
	class BoxCollider : public Collider
	{
	public:
		Box shape;

		BoxCollider( const Box& shape );

		//  TODO: implement this
		bool intersects( std::shared_ptr<Collider> other ) override;
		//  source: https://subscription.packtpub.com/book/game+development/9781787123663/14/ch14lvl1sec135/raycast-bounding-box
		bool raycast( _RAYCAST_FUNC_PARAMS );

		void debug_render( RenderBatch* render_batch ) override;
	};
}