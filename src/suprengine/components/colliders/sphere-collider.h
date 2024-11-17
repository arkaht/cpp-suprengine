#pragma once

#include <suprengine/components/collider.h>

namespace suprengine
{
	class SphereCollider : public Collider
	{
	public:
		float radius { 0.0f };

		SphereCollider( float radius );

		//  TODO: implement this
		bool intersects( SharedPtr<Collider> other ) override;
		//  source: https://gdbooks.gitbooks.io/3dcollisions/content/Chapter3/raycast_sphere.html
		bool raycast( _RAYCAST_FUNC_PARAMS );

		float get_scaled_radius() const;

		void debug_render( RenderBatch* render_batch ) override;
	};
}