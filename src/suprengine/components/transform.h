#pragma once

#include <suprengine/core/component.h>

#include <suprengine/math/rect.h>
#include <suprengine/math/mtx4.h>

namespace suprengine
{
	/*
	 * Component handling the location, rotation and scale of an entity.
	 * 
	 * It exposes vectors and quaternion for the user to manipulate
	 * and packs them into a matrix for rendering purposes.
	 * 
	 * Currently, there is a known architecture issue where setting directly
	 * any of the variables after entity's first rendering will NOT update the 
	 * component's matrix. For now, you should use the setter functions for 
	 * these purposes.
	 */
	class Transform : public Component
	{
	public:
		/*
		 * Deprecated. It was designed for the old 2D engine.
		 * TODO: Remove
		 */
		Rect get_rect( const Rect& rect );

		void set_location( const Vec2& vec );
		void set_location( const Vec3& vec );

		void set_rotation( const Quaternion& quat );

		void set_scale( const Vec2& vec );
		void set_scale( const Vec3& vec );

		/*
		 * Finds and sets a rotation from the component's location towards
		 * a given location.
		 */
		void look_at( const Vec3& target );

		Vec3 get_forward() const;
		Vec3 get_right() const;
		Vec3 get_up() const;

		/*
		 * Updates the matrix if dirty and returns it.
		 */
		const Mtx4& get_matrix();
		bool is_matrix_dirty() const;

	public:
		/*
		 * Location of the transform.
		 * 
		 * Don't set this variable directly, use the function set_location
		 * or the component's matrix won't be updated.
		 */
		Vec3 location = Vec3::zero;
		/*
		 * Rotation of the transform.
		 * 
		 * Don't set this variable directly, use the function set_rotation
		 * or the component's matrix won't be updated.
		 */
		Quaternion rotation = Quaternion::identity;
		/*
		 * Scale of the transform.
		 * 
		 * Don't set this variable directly, use the function set_scale
		 * or the component's matrix won't be updated.
		 */
		Vec3 scale = Vec3::one;

	private:
		Mtx4 _matrix {};
		bool _is_matrix_dirty = true;
	};
}