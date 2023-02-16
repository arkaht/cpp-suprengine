#pragma once

#include <suprengine/ecs/component.h>
#include <suprengine/mtx4.h>

namespace suprengine
{
	class Transform3 : Component
	{
	private:
		Mtx4 matrix;
		bool is_matrix_dirty { true };

	public:
		Transform3( Spatial* owner, int priority_order = 0 )
			: Component( owner, priority_order ) {}

		Vec3 location;
		Quaternion rotation;
		Vec3 scale { Vec3::one };

		void set_location( const Vec3& vec ) { location = vec; mark_dirty(); }
		void set_rotation( const Quaternion quat ) { rotation = quat; mark_dirty(); }
		void set_scale( const Vec3 vec ) { scale = vec; mark_dirty(); }

		Vec3 get_forward() const { return Vec3::transform( Vec3::unitX, rotation ); }
		Vec3 get_right() const { return Vec3::transform( Vec3::unitZ, rotation ); }
		Vec3 get_up() const { return Vec3::transform( Vec3::unitY, rotation ); }

		void mark_dirty() { is_matrix_dirty = true; }
		const Mtx4& get_matrix()
		{
			if ( is_matrix_dirty )
			{
				matrix = Mtx4::create_scale( scale )
					   * Mtx4::create_from_quaternion( rotation ) 
					   * Mtx4::create_translation( location );

				is_matrix_dirty = false;
			}

			return matrix;
		}
	};
}