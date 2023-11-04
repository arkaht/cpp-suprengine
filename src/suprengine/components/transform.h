#pragma once
#include "transform.fwd.h"

#include <suprengine/component.h>
#include <suprengine/mtx4.h>

namespace suprengine
{
	class Transform : public Component
	{
	public:
		bool is_matrix_dirty { true };

		Transform( Entity* owner ) : Component( owner ) {}

		Vec3 location;
		Quaternion rotation;
		Vec3 scale { Vec3::one };

		Rect get_rect( const Rect& rect );

		void set_location( const Vec2& vec );
		void set_location( const Vec3& vec );

		void set_rotation( const Quaternion& quat );

		void set_scale( const Vec2& vec );
		void set_scale( const Vec3& vec );

		void look_at( const Vec3& target );

		Vec3 get_forward() const { return Vec3::transform( Vec3::forward, rotation ); }
		Vec3 get_right() const { return Vec3::transform( Vec3::right, rotation ); }
		Vec3 get_up() const { return Vec3::transform( Vec3::up, rotation ); }

		const Mtx4& get_matrix();

	private:
		Mtx4 matrix;
	};
}