#pragma once
#include "transform.fwd.h"

#include <suprengine/ecs/component.h>
#include <suprengine/mtx4.h>

namespace suprengine
{
	class Transform : Component
	{
	private:
		Mtx4 matrix;

	public:
		bool is_matrix_dirty { true };

		Transform( Entity* owner ) : Component( owner ) {}

		Vec3 location;
		Quaternion rotation;
		Vec3 scale { Vec3::one };

		Rect get_rect( const Rect& rect )
		{
			return {
				location.x + rect.x * scale.x,
				location.y + rect.y * scale.y,
				rect.w * scale.x,
				rect.h * scale.y
			};
		}

		void set_location( const Vec2& vec )
		{
			location.x = vec.x, location.y = vec.y, location.z = 0.0f;
			is_matrix_dirty = true;
		}
		void set_location( const Vec3& vec )
		{
			location = vec;
			is_matrix_dirty = true;
		}

		void set_rotation( const Quaternion& quat )
		{
			rotation = quat;
			is_matrix_dirty = true;
		}

		void set_scale( const Vec2& vec )
		{
			scale.x = vec.x, scale.y = vec.y, scale.z = 1.0f;
			is_matrix_dirty = true;
		}
		void set_scale( const Vec3& vec ) 
		{ 
			scale = vec; 
			is_matrix_dirty = true;
		}

		void look_at( const Vec3& target )
		{
			set_rotation(
				Quaternion::look_at(
					location,
					target,
					Vec3::forward,
					Vec3::up
				)
			);
		}

		Vec3 get_forward() const { return Vec3::transform( Vec3::forward, rotation ); }
		Vec3 get_right() const { return Vec3::transform( Vec3::right, rotation ); }
		Vec3 get_up() const { return Vec3::transform( Vec3::up, rotation ); }

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