#pragma once

#include <suprengine/vec3.h>
#include <suprengine/quaternion.h>

namespace suprengine
{
	struct Mtx4
	{
	public:
		static const Mtx4 identity;

	public:
		float mat[4][4] {};

	public:
		Mtx4();
		explicit Mtx4( float array[4][4] );

	public:
		void invert();

		Mtx4 inverse() const;	

		Vec3 get_translation() const;

		Vec3 get_x_axis() const;
		Vec3 get_y_axis() const;
		Vec3 get_z_axis() const;

		Vec3 get_scale() const;

	public:
		static Mtx4 create_from_transform(
			const Vec3& scale,
			const Quaternion& rotation,
			const Vec3& location
		);

		static Mtx4 create_scale( float x, float y, float z );
		static Mtx4 create_scale( const Vec3& scale );
		static Mtx4 create_scale( float scale );

		static Mtx4 create_rotation_x( float theta );
		static Mtx4 create_rotation_y( float theta );
		static Mtx4 create_rotation_z( float theta );

		static Mtx4 create_from_quaternion( const Quaternion& q );
		static Mtx4 create_look_at( const Vec3& eye, const Vec3& target, const Vec3& up );

		static Mtx4 create_translation( float x, float y, float z );
		static Mtx4 create_translation( const Vec3& pos );

		static Mtx4 create_ortho( float width, float height, float near, float far );
		static Mtx4 create_perspective_fov( float fovY, float width, float height, float near, float far );
		static Mtx4 create_simple_view_projection( float width, float height );

	public:
		//  Matrix multiplication (a * b)
		friend Mtx4 operator*( const Mtx4& a, const Mtx4& b )
		{
			Mtx4 retVal;

			//  Row 0
			retVal.mat[0][0] =
				a.mat[0][0] * b.mat[0][0] +
				a.mat[0][1] * b.mat[1][0] +
				a.mat[0][2] * b.mat[2][0] +
				a.mat[0][3] * b.mat[3][0];

			retVal.mat[0][1] =
				a.mat[0][0] * b.mat[0][1] +
				a.mat[0][1] * b.mat[1][1] +
				a.mat[0][2] * b.mat[2][1] +
				a.mat[0][3] * b.mat[3][1];

			retVal.mat[0][2] =
				a.mat[0][0] * b.mat[0][2] +
				a.mat[0][1] * b.mat[1][2] +
				a.mat[0][2] * b.mat[2][2] +
				a.mat[0][3] * b.mat[3][2];

			retVal.mat[0][3] =
				a.mat[0][0] * b.mat[0][3] +
				a.mat[0][1] * b.mat[1][3] +
				a.mat[0][2] * b.mat[2][3] +
				a.mat[0][3] * b.mat[3][3];

			//  Row 1
			retVal.mat[1][0] =
				a.mat[1][0] * b.mat[0][0] +
				a.mat[1][1] * b.mat[1][0] +
				a.mat[1][2] * b.mat[2][0] +
				a.mat[1][3] * b.mat[3][0];

			retVal.mat[1][1] =
				a.mat[1][0] * b.mat[0][1] +
				a.mat[1][1] * b.mat[1][1] +
				a.mat[1][2] * b.mat[2][1] +
				a.mat[1][3] * b.mat[3][1];

			retVal.mat[1][2] =
				a.mat[1][0] * b.mat[0][2] +
				a.mat[1][1] * b.mat[1][2] +
				a.mat[1][2] * b.mat[2][2] +
				a.mat[1][3] * b.mat[3][2];

			retVal.mat[1][3] =
				a.mat[1][0] * b.mat[0][3] +
				a.mat[1][1] * b.mat[1][3] +
				a.mat[1][2] * b.mat[2][3] +
				a.mat[1][3] * b.mat[3][3];

			//  Row 2
			retVal.mat[2][0] =
				a.mat[2][0] * b.mat[0][0] +
				a.mat[2][1] * b.mat[1][0] +
				a.mat[2][2] * b.mat[2][0] +
				a.mat[2][3] * b.mat[3][0];

			retVal.mat[2][1] =
				a.mat[2][0] * b.mat[0][1] +
				a.mat[2][1] * b.mat[1][1] +
				a.mat[2][2] * b.mat[2][1] +
				a.mat[2][3] * b.mat[3][1];

			retVal.mat[2][2] =
				a.mat[2][0] * b.mat[0][2] +
				a.mat[2][1] * b.mat[1][2] +
				a.mat[2][2] * b.mat[2][2] +
				a.mat[2][3] * b.mat[3][2];

			retVal.mat[2][3] =
				a.mat[2][0] * b.mat[0][3] +
				a.mat[2][1] * b.mat[1][3] +
				a.mat[2][2] * b.mat[2][3] +
				a.mat[2][3] * b.mat[3][3];

			//  Row 3
			retVal.mat[3][0] =
				a.mat[3][0] * b.mat[0][0] +
				a.mat[3][1] * b.mat[1][0] +
				a.mat[3][2] * b.mat[2][0] +
				a.mat[3][3] * b.mat[3][0];

			retVal.mat[3][1] =
				a.mat[3][0] * b.mat[0][1] +
				a.mat[3][1] * b.mat[1][1] +
				a.mat[3][2] * b.mat[2][1] +
				a.mat[3][3] * b.mat[3][1];

			retVal.mat[3][2] =
				a.mat[3][0] * b.mat[0][2] +
				a.mat[3][1] * b.mat[1][2] +
				a.mat[3][2] * b.mat[2][2] +
				a.mat[3][3] * b.mat[3][2];

			retVal.mat[3][3] =
				a.mat[3][0] * b.mat[0][3] +
				a.mat[3][1] * b.mat[1][3] +
				a.mat[3][2] * b.mat[2][3] +
				a.mat[3][3] * b.mat[3][3];

			return retVal;
		}
		Mtx4& operator*=( const Mtx4& right )
		{
			*this = *this * right;
			return *this;
		}

		float* operator[]( int id )
		{
			return mat[id];
		}
		const float* operator[]( int id ) const
		{
			return mat[id];
		}
	};
}