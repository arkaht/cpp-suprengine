#pragma once

#include <suprengine/ecs/component.h>
#include <suprengine/mtx4.h>

namespace suprengine
{
	class Transform3 : Component
	{
	private:
		bool is_matrix_dirty { true };
	public:
		Mtx4 world_transform;
		Vec3 location;
		Quaternion rotation;
		float scale { 1.0f };

		void update( float dt ) override
		{
			if ( is_matrix_dirty )
				compute_world_transform();
		}

		void mark_dirty() { is_matrix_dirty = true; }

		void compute_world_transform()
		{
			world_transform = Mtx4::create_scale( scale ) 
							* Mtx4::create_from_quaternion( rotation ) 
							* Mtx4::create_translation( location );

			is_matrix_dirty = false;
		}
	};
}