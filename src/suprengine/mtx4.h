#pragma once

#include <suprengine/vec3.h>
#include <suprengine/quaternion.h>

namespace suprengine
{
	struct Mtx4
	{
	public:
		float mat[4][4] {};

		Mtx4() {}

		explicit Mtx4( float inMat[4][4] )
		{
			memcpy( mat, inMat, 16 * sizeof( float ) );
		}

		// Cast to a const float pointer
		const float* get_as_float_pointer() const
		{
			return reinterpret_cast<const float*>( &mat[0][0] );
		}

		// Matrix multiplication (a * b)
		friend Mtx4 operator*( const Mtx4& a, const Mtx4& b )
		{
			Mtx4 retVal;
			// row 0
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

			// row 1
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

			// row 2
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

			// row 3
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

		void invert();

		Mtx4 inverse() const;

		Vec3 get_translation() const
		{
			return Vec3( mat[3][0], mat[3][1], mat[3][2] );
		}

		Vec3 get_x_axis() const
		{
			return Vec3::normalize( Vec3( mat[0][0], mat[0][1], mat[0][2] ) );
		}
		Vec3 get_y_axis() const
		{
			return Vec3::normalize( Vec3( mat[1][0], mat[1][1], mat[1][2] ) );
		}
		Vec3 get_z_axis() const
		{
			return Vec3::normalize( Vec3( mat[2][0], mat[2][1], mat[2][2] ) );
		}

		Vec3 get_scale() const
		{
			Vec3 retVal;
			retVal.x = Vec3( mat[0][0], mat[0][1], mat[0][2] ).length();
			retVal.y = Vec3( mat[1][0], mat[1][1], mat[1][2] ).length();
			retVal.z = Vec3( mat[2][0], mat[2][1], mat[2][2] ).length();
			return retVal;
		}

		static Mtx4 create_from_transform( 
			const Vec3& scale, 
			const Quaternion& rotation, 
			const Vec3& location 
		);

		static Mtx4 create_scale( float xScale, float yScale, float zScale )
		{
			float temp[4][4] =
			{
				{ xScale, 0.0f, 0.0f, 0.0f },
				{ 0.0f, yScale, 0.0f, 0.0f },
				{ 0.0f, 0.0f, zScale, 0.0f },
				{ 0.0f, 0.0f, 0.0f, 1.0f }
			};
			return Mtx4( temp );
		}
		static Mtx4 create_scale( const Vec3& scaleVector )
		{
			return create_scale( scaleVector.x, scaleVector.y, scaleVector.z );
		}
		static Mtx4 create_scale( float scale )
		{
			return create_scale( scale, scale, scale );
		}

		static Mtx4 create_rotation_x( float theta )
		{
			float temp[4][4] =
			{
				{ 1.0f, 0.0f, 0.0f , 0.0f },
				{ 0.0f, math::cos( theta ), math::sin( theta ), 0.0f },
				{ 0.0f, math::sin( theta ), math::cos( theta ), 0.0f },
				{ 0.0f, 0.0f, 0.0f, 1.0f },
			};
			return Mtx4( temp );
		}
		static Mtx4 create_rotation_y( float theta )
		{
			float temp[4][4] =
			{
				{ math::cos( theta ), 0.0f, -math::sin( theta ), 0.0f },
				{ 0.0f, 1.0f, 0.0f, 0.0f },
				{ math::sin( theta ), 0.0f, math::cos( theta ), 0.0f },
				{ 0.0f, 0.0f, 0.0f, 1.0f },
			};
			return Mtx4( temp );
		}
		static Mtx4 create_rotation_z( float theta )
		{
			float temp[4][4] =
			{
				{ math::cos( theta ), math::sin( theta ), 0.0f, 0.0f },
				{ -math::sin( theta ), math::cos( theta ), 0.0f, 0.0f },
				{ 0.0f, 0.0f, 1.0f, 0.0f },
				{ 0.0f, 0.0f, 0.0f, 1.0f },
			};
			return Mtx4( temp );
		}

		static Mtx4 create_from_quaternion( const Quaternion& q )
		{
			float mat[4][4];

			mat[0][0] = 1.0f - 2.0f * q.y * q.y - 2.0f * q.z * q.z;
			mat[0][1] = 2.0f * q.x * q.y + 2.0f * q.w * q.z;
			mat[0][2] = 2.0f * q.x * q.z - 2.0f * q.w * q.y;
			mat[0][3] = 0.0f;

			mat[1][0] = 2.0f * q.x * q.y - 2.0f * q.w * q.z;
			mat[1][1] = 1.0f - 2.0f * q.x * q.x - 2.0f * q.z * q.z;
			mat[1][2] = 2.0f * q.y * q.z + 2.0f * q.w * q.x;
			mat[1][3] = 0.0f;

			mat[2][0] = 2.0f * q.x * q.z + 2.0f * q.w * q.y;
			mat[2][1] = 2.0f * q.y * q.z - 2.0f * q.w * q.x;
			mat[2][2] = 1.0f - 2.0f * q.x * q.x - 2.0f * q.y * q.y;
			mat[2][3] = 0.0f;

			mat[3][0] = 0.0f;
			mat[3][1] = 0.0f;
			mat[3][2] = 0.0f;
			mat[3][3] = 1.0f;

			return Mtx4( mat );
		}
		static Mtx4 create_look_at( const Vec3& eye, const Vec3& target, const Vec3& up )
		{
			Vec3 zaxis = Vec3::normalize( target - eye );
			Vec3 xaxis = Vec3::normalize( Vec3::cross( up, zaxis ) );
			Vec3 yaxis = Vec3::normalize( Vec3::cross( zaxis, xaxis ) );
			Vec3 trans;
			trans.x = -Vec3::dot( xaxis, eye );
			trans.y = -Vec3::dot( yaxis, eye );
			trans.z = -Vec3::dot( zaxis, eye );

			float temp[4][4] =
			{
				{ xaxis.x, yaxis.x, zaxis.x, 0.0f },
				{ xaxis.y, yaxis.y, zaxis.y, 0.0f },
				{ xaxis.z, yaxis.z, zaxis.z, 0.0f },
				{ trans.x, trans.y, trans.z, 1.0f }
			};
			return Mtx4( temp );
		}

		static Mtx4 create_translation( float x, float y, float z )
		{
			float temp[4][4] =
			{
				{ 1.0f, 0.0f, 0.0f, 0.0f },
				{ 0.0f, 1.0f, 0.0f, 0.0f },
				{ 0.0f, 0.0f, 1.0f, 0.0f },
				{ x, y, z, 1.0f }
			};
			return Mtx4( temp );
		}
		static Mtx4 create_translation( const Vec3& pos )
		{
			return create_translation( pos.x, pos.y, pos.z );
		}

		static Mtx4 create_ortho( float width, float height, float near, float far )
		{
			float temp[4][4] =
			{
				{ 2.0f / width, 0.0f, 0.0f, 0.0f },
				{ 0.0f, 2.0f / height, 0.0f, 0.0f },
				{ 0.0f, 0.0f, 1.0f / ( far - near ), 0.0f },
				{ 0.0f, 0.0f, near / ( near - far ), 1.0f }
			};
			return Mtx4( temp );
		}
		static Mtx4 create_perspective_fov( float fovY, float width, float height, float near, float far )
		{
			float yScale = math::cot( fovY / 2.0f );
			float xScale = yScale * height / width;
			float temp[4][4] =
			{
				{ xScale, 0.0f, 0.0f, 0.0f },
				{ 0.0f, yScale, 0.0f, 0.0f },
				{ 0.0f, 0.0f, far / ( far - near ), 1.0f },
				{ 0.0f, 0.0f, -near * far / ( far - near ), 0.0f }
			};
			return Mtx4( temp );
		}
		static Mtx4 create_simple_view_projection( float width, float height )
		{
			float temp[4][4] =
			{
				{ 2.0f / width, 0.0f, 0.0f, 0.0f },
				{ 0.0f, 2.0f / height, 0.0f, 0.0f },
				{ 0.0f, 0.0f, 1.0f, 0.0f },
				{ 0.0f, 0.0f, 1.0f, 1.0f }
			};
			return Mtx4( temp );
		}

		static const Mtx4 identity;
	};
}