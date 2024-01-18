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

		Vec3 location { Vec3::zero };
		Quaternion rotation { Quaternion::identity };
		Vec3 scale { Vec3::one };

	public:
		Transform( Entity* owner ) : Component( owner ) {}

		Rect get_rect( const Rect& rect );

		void set_location( const Vec2& vec );
		void set_location( const Vec3& vec );

		void set_rotation( const Quaternion& quat );

		void set_scale( const Vec2& vec );
		void set_scale( const Vec3& vec );

		void look_at( const Vec3& target );

		Vec3 get_forward() const;
		Vec3 get_right() const;
		Vec3 get_up() const;

		const Mtx4& get_matrix();

	private:
		Mtx4 matrix;
	};
}