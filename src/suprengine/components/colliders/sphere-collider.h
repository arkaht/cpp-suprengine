#pragma once

#include <suprengine/components/collider.h>
#include <suprengine/components/transform.h>

#include <suprengine/assets.h>

namespace suprengine
{
	class SphereCollider : public Collider
	{
	public:
		float radius { 0.0f };

		SphereCollider( float radius );;

		//  TODO: implement this
		bool intersects( SharedPtr<Collider> other ) override;
		//  source: https://gdbooks.gitbooks.io/3dcollisions/content/Chapter3/raycast_sphere.html
		bool raycast( _RAYCAST_FUNC_PARAMS );

		void debug_render( RenderBatch* render_batch ) override;
	};
}