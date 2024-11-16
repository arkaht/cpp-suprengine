#include "mtx4.h"

using namespace suprengine;

static float m4Ident[4][4] {
	{ 1.0f, 0.0f, 0.0f, 0.0f },
	{ 0.0f, 1.0f, 0.0f, 0.0f },
	{ 0.0f, 0.0f, 1.0f, 0.0f },
	{ 0.0f, 0.0f, 0.0f, 1.0f }
};

const Mtx4 Mtx4::identity( m4Ident );

Mtx4::Mtx4()
{
	*this = Mtx4::identity;
}

Mtx4::Mtx4( float array[4][4] )
{
	memcpy( mat, array, 16 * sizeof( float ) );
}

void Mtx4::invert()
{
	//	Code taken from:
	//	https://stackoverflow.com/a/44446912

	const float A2323 = mat[2][2] * mat[3][3] - mat[2][3] * mat[3][2];
	const float A1323 = mat[2][1] * mat[3][3] - mat[2][3] * mat[3][1];
	const float A1223 = mat[2][1] * mat[3][2] - mat[2][2] * mat[3][1];
	const float A0323 = mat[2][0] * mat[3][3] - mat[2][3] * mat[3][0];
	const float A0223 = mat[2][0] * mat[3][2] - mat[2][2] * mat[3][0];
	const float A0123 = mat[2][0] * mat[3][1] - mat[2][1] * mat[3][0];
	const float A2313 = mat[1][2] * mat[3][3] - mat[1][3] * mat[3][2];
	const float A1313 = mat[1][1] * mat[3][3] - mat[1][3] * mat[3][1];
	const float A1213 = mat[1][1] * mat[3][2] - mat[1][2] * mat[3][1];
	const float A2312 = mat[1][2] * mat[2][3] - mat[1][3] * mat[2][2];
	const float A1312 = mat[1][1] * mat[2][3] - mat[1][3] * mat[2][1];
	const float A1212 = mat[1][1] * mat[2][2] - mat[1][2] * mat[2][1];
	const float A0313 = mat[1][0] * mat[3][3] - mat[1][3] * mat[3][0];
	const float A0213 = mat[1][0] * mat[3][2] - mat[1][2] * mat[3][0];
	const float A0312 = mat[1][0] * mat[2][3] - mat[1][3] * mat[2][0];
	const float A0212 = mat[1][0] * mat[2][2] - mat[1][2] * mat[2][0];
	const float A0113 = mat[1][0] * mat[3][1] - mat[1][1] * mat[3][0];
	const float A0112 = mat[1][0] * mat[2][1] - mat[1][1] * mat[2][0];

	float det = mat[0][0] * ( mat[1][1] * A2323 - mat[1][2] * A1323 + mat[1][3] * A1223 ) 
			  - mat[0][1] * ( mat[1][0] * A2323 - mat[1][2] * A0323 + mat[1][3] * A0223 ) 
			  + mat[0][2] * ( mat[1][0] * A1323 - mat[1][1] * A0323 + mat[1][3] * A0123 ) 
			  - mat[0][3] * ( mat[1][0] * A1223 - mat[1][1] * A0223 + mat[1][2] * A0123 );
	det = 1.0f / det;

	//	NOTE: We still need a copy of the original matrix to invert
	const Mtx4 source = *this;

	mat[0][0] = det *   ( source[1][1] * A2323 - source[1][2] * A1323 + source[1][3] * A1223 );
	mat[0][1] = det * - ( source[0][1] * A2323 - source[0][2] * A1323 + source[0][3] * A1223 );
	mat[0][2] = det *   ( source[0][1] * A2313 - source[0][2] * A1313 + source[0][3] * A1213 );
	mat[0][3] = det * - ( source[0][1] * A2312 - source[0][2] * A1312 + source[0][3] * A1212 );

	mat[1][0] = det * - ( source[1][0] * A2323 - source[1][2] * A0323 + source[1][3] * A0223 );
	mat[1][1] = det *   ( source[0][0] * A2323 - source[0][2] * A0323 + source[0][3] * A0223 );
	mat[1][2] = det * - ( source[0][0] * A2313 - source[0][2] * A0313 + source[0][3] * A0213 );
	mat[1][3] = det *   ( source[0][0] * A2312 - source[0][2] * A0312 + source[0][3] * A0212 );
	
	mat[2][0] = det *   ( source[1][0] * A1323 - source[1][1] * A0323 + source[1][3] * A0123 );
	mat[2][1] = det * - ( source[0][0] * A1323 - source[0][1] * A0323 + source[0][3] * A0123 );
	mat[2][2] = det *   ( source[0][0] * A1313 - source[0][1] * A0313 + source[0][3] * A0113 );
	mat[2][3] = det * - ( source[0][0] * A1312 - source[0][1] * A0312 + source[0][3] * A0112 );
	
	mat[3][0] = det * - ( source[1][0] * A1223 - source[1][1] * A0223 + source[1][2] * A0123 );
	mat[3][1] = det *   ( source[0][0] * A1223 - source[0][1] * A0223 + source[0][2] * A0123 );
	mat[3][2] = det * - ( source[0][0] * A1213 - source[0][1] * A0213 + source[0][2] * A0113 );
	mat[3][3] = det *   ( source[0][0] * A1212 - source[0][1] * A0212 + source[0][2] * A0112 );
}

Mtx4 Mtx4::inverse() const
{
	//	Code taken from:
	//	https://stackoverflow.com/a/44446912

	const float A2323 = mat[2][2] * mat[3][3] - mat[2][3] * mat[3][2];
	const float A1323 = mat[2][1] * mat[3][3] - mat[2][3] * mat[3][1];
	const float A1223 = mat[2][1] * mat[3][2] - mat[2][2] * mat[3][1];
	const float A0323 = mat[2][0] * mat[3][3] - mat[2][3] * mat[3][0];
	const float A0223 = mat[2][0] * mat[3][2] - mat[2][2] * mat[3][0];
	const float A0123 = mat[2][0] * mat[3][1] - mat[2][1] * mat[3][0];
	const float A2313 = mat[1][2] * mat[3][3] - mat[1][3] * mat[3][2];
	const float A1313 = mat[1][1] * mat[3][3] - mat[1][3] * mat[3][1];
	const float A1213 = mat[1][1] * mat[3][2] - mat[1][2] * mat[3][1];
	const float A2312 = mat[1][2] * mat[2][3] - mat[1][3] * mat[2][2];
	const float A1312 = mat[1][1] * mat[2][3] - mat[1][3] * mat[2][1];
	const float A1212 = mat[1][1] * mat[2][2] - mat[1][2] * mat[2][1];
	const float A0313 = mat[1][0] * mat[3][3] - mat[1][3] * mat[3][0];
	const float A0213 = mat[1][0] * mat[3][2] - mat[1][2] * mat[3][0];
	const float A0312 = mat[1][0] * mat[2][3] - mat[1][3] * mat[2][0];
	const float A0212 = mat[1][0] * mat[2][2] - mat[1][2] * mat[2][0];
	const float A0113 = mat[1][0] * mat[3][1] - mat[1][1] * mat[3][0];
	const float A0112 = mat[1][0] * mat[2][1] - mat[1][1] * mat[2][0];

	float det = mat[0][0] * ( mat[1][1] * A2323 - mat[1][2] * A1323 + mat[1][3] * A1223 ) 
			  - mat[0][1] * ( mat[1][0] * A2323 - mat[1][2] * A0323 + mat[1][3] * A0223 ) 
			  + mat[0][2] * ( mat[1][0] * A1323 - mat[1][1] * A0323 + mat[1][3] * A0123 ) 
			  - mat[0][3] * ( mat[1][0] * A1223 - mat[1][1] * A0223 + mat[1][2] * A0123 );
	det = 1.0f / det;

	Mtx4 matrix {};
	matrix[0][0] = det *   ( mat[1][1] * A2323 - mat[1][2] * A1323 + mat[1][3] * A1223 );
	matrix[0][1] = det * - ( mat[0][1] * A2323 - mat[0][2] * A1323 + mat[0][3] * A1223 );
	matrix[0][2] = det *   ( mat[0][1] * A2313 - mat[0][2] * A1313 + mat[0][3] * A1213 );
	matrix[0][3] = det * - ( mat[0][1] * A2312 - mat[0][2] * A1312 + mat[0][3] * A1212 );
	
	matrix[1][0] = det * - ( mat[1][0] * A2323 - mat[1][2] * A0323 + mat[1][3] * A0223 );
	matrix[1][1] = det *   ( mat[0][0] * A2323 - mat[0][2] * A0323 + mat[0][3] * A0223 );
	matrix[1][2] = det * - ( mat[0][0] * A2313 - mat[0][2] * A0313 + mat[0][3] * A0213 );
	matrix[1][3] = det *   ( mat[0][0] * A2312 - mat[0][2] * A0312 + mat[0][3] * A0212 );
	
	matrix[2][0] = det *   ( mat[1][0] * A1323 - mat[1][1] * A0323 + mat[1][3] * A0123 );
	matrix[2][1] = det * - ( mat[0][0] * A1323 - mat[0][1] * A0323 + mat[0][3] * A0123 );
	matrix[2][2] = det *   ( mat[0][0] * A1313 - mat[0][1] * A0313 + mat[0][3] * A0113 );
	matrix[2][3] = det * - ( mat[0][0] * A1312 - mat[0][1] * A0312 + mat[0][3] * A0112 );
	
	matrix[3][0] = det * - ( mat[1][0] * A1223 - mat[1][1] * A0223 + mat[1][2] * A0123 );
	matrix[3][1] = det *   ( mat[0][0] * A1223 - mat[0][1] * A0223 + mat[0][2] * A0123 );
	matrix[3][2] = det * - ( mat[0][0] * A1213 - mat[0][1] * A0213 + mat[0][2] * A0113 );
	matrix[3][3] = det *   ( mat[0][0] * A1212 - mat[0][1] * A0212 + mat[0][2] * A0112 );

	return matrix;
}

Vec3 Mtx4::get_translation() const
{
	return Vec3( mat[3][0], mat[3][1], mat[3][2] );
}

Vec3 Mtx4::get_x_axis() const
{
	return Vec3( mat[0][0], mat[0][1], mat[0][2] ).normalized();
}

Vec3 Mtx4::get_y_axis() const
{
	return Vec3( mat[1][0], mat[1][1], mat[1][2] ).normalized();
}

Vec3 Mtx4::get_z_axis() const
{
	return Vec3( mat[2][0], mat[2][1], mat[2][2] ).normalized();
}

Vec3 Mtx4::get_scale() const
{
	return Vec3 {
		Vec3( mat[0][0], mat[0][1], mat[0][2] ).length(),
		Vec3( mat[1][0], mat[1][1], mat[1][2] ).length(),
		Vec3( mat[2][0], mat[2][1], mat[2][2] ).length(),
	};
}

Mtx4 Mtx4::create_from_transform(
	const Vec3& scale, 
	const Quaternion& rotation,
	const Vec3& location 
)
{
	return Mtx4::create_scale( scale )
		 * Mtx4::create_from_quaternion( rotation )
		 * Mtx4::create_translation( location );
}

Mtx4 Mtx4::create_scale( float x, float y, float z )
{
	float temp[4][4] {
		{ x,	0.0f, 0.0f, 0.0f },
		{ 0.0f, y,	  0.0f, 0.0f },
		{ 0.0f, 0.0f, z,	0.0f },
		{ 0.0f, 0.0f, 0.0f,	1.0f },
	};
	return Mtx4( temp );
}

Mtx4 Mtx4::create_scale( const Vec3& scale )
{
	return create_scale( scale.x, scale.y, scale.z );
}

Mtx4 Mtx4::create_scale( float scale )
{
	return create_scale( scale, scale, scale );
}

Mtx4 Mtx4::create_rotation_x( float theta )
{
	float cosinus = math::cos( theta );
	float sinus = math::sin( theta );

	float temp[4][4] {
		{ 1.0f, 0.0f,	 0.0f,	  0.0f },
		{ 0.0f, cosinus, sinus,	  0.0f },
		{ 0.0f, sinus,	 cosinus, 0.0f },
		{ 0.0f, 0.0f,	 0.0f,	  1.0f },
	};
	return Mtx4( temp );
}

Mtx4 Mtx4::create_rotation_y( float theta )
{
	const float cosinus = math::cos( theta );
	const float sinus = math::sin( theta );
	
	float temp[4][4] {
		{ cosinus, 0.0f, -sinus,  0.0f },
		{ 0.0f,	   1.0f, 0.0f,    0.0f },
		{ sinus,   0.0f, cosinus, 0.0f },
		{ 0.0f,    0.0f, 0.0f,	  1.0f },
	};
	return Mtx4( temp );
}

Mtx4 Mtx4::create_rotation_z( float theta )
{
	const float cosinus = math::cos( theta );
	const float sinus = math::sin( theta );

	float temp[4][4] =
	{
		{ cosinus,	sinus,	 0.0f, 0.0f },
		{ -sinus,	cosinus, 0.0f, 0.0f },
		{ 0.0f,		0.0f,	 1.0f, 0.0f },
		{ 0.0f,		0.0f,	 0.0f, 1.0f },
	};
	return Mtx4( temp );
}

Mtx4 Mtx4::create_from_quaternion( const Quaternion& q )
{
	Mtx4 matrix {};

	matrix[0][0] = 1.0f - 2.0f * q.y * q.y - 2.0f * q.z * q.z;
	matrix[0][1] = 2.0f * q.x * q.y + 2.0f * q.w * q.z;
	matrix[0][2] = 2.0f * q.x * q.z - 2.0f * q.w * q.y;

	matrix[1][0] = 2.0f * q.x * q.y - 2.0f * q.w * q.z;
	matrix[1][1] = 1.0f - 2.0f * q.x * q.x - 2.0f * q.z * q.z;
	matrix[1][2] = 2.0f * q.y * q.z + 2.0f * q.w * q.x;

	matrix[2][0] = 2.0f * q.x * q.z + 2.0f * q.w * q.y;
	matrix[2][1] = 2.0f * q.y * q.z - 2.0f * q.w * q.x;
	matrix[2][2] = 1.0f - 2.0f * q.x * q.x - 2.0f * q.y * q.y;

	//	NOTE: We omit some values in the Matrix since they are already
	//	initialized to identity.

	return matrix;
}

Mtx4 Mtx4::create_look_at( const Vec3& eye, const Vec3& target, const Vec3& up )
{
	Vec3 zaxis = Vec3::normalize( target - eye );
	Vec3 xaxis = Vec3::normalize( Vec3::cross( up, zaxis ) );
	Vec3 yaxis = Vec3::normalize( Vec3::cross( zaxis, xaxis ) );

	Vec3 trans {
		-Vec3::dot( xaxis, eye ),
		-Vec3::dot( yaxis, eye ),
		-Vec3::dot( zaxis, eye ),
	};

	float temp[4][4] =
	{
		{ xaxis.x, yaxis.x, zaxis.x, 0.0f },
		{ xaxis.y, yaxis.y, zaxis.y, 0.0f },
		{ xaxis.z, yaxis.z, zaxis.z, 0.0f },
		{ trans.x, trans.y, trans.z, 1.0f },
	};
	return Mtx4( temp );
}

Mtx4 Mtx4::create_translation( float x, float y, float z )
{
	float temp[4][4] {
		{ 1.0f, 0.0f, 0.0f, 0.0f },
		{ 0.0f, 1.0f, 0.0f, 0.0f },
		{ 0.0f, 0.0f, 1.0f, 0.0f },
		{ x,	y,	  z,	1.0f },
	};
	return Mtx4( temp );
}

Mtx4 Mtx4::create_translation( const Vec3& pos )
{
	return create_translation( pos.x, pos.y, pos.z );
}

Mtx4 Mtx4::create_ortho( float width, float height, float near, float far )
{
	float temp[4][4] {
		{ 2.0f / width, 0.0f,		   0.0f,				  0.0f },
		{ 0.0f,			2.0f / height, 0.0f,				  0.0f },
		{ 0.0f,			0.0f,		   1.0f / ( far - near ), 0.0f },
		{ 0.0f,			0.0f,		   near / ( near - far ), 1.0f },
	};
	return Mtx4( temp );
}

Mtx4 Mtx4::create_perspective_fov( float fovY, float width, float height, float near, float far )
{
	float scale_y = math::cot( fovY / 2.0f );
	float scale_x = scale_y * height / width;
	float temp[4][4] {
		{ scale_x, 0.0f,	0.0f,						  0.0f },
		{ 0.0f,	   scale_y, 0.0f,						  0.0f },
		{ 0.0f,    0.0f,	far / ( far - near ),		  1.0f },
		{ 0.0f,    0.0f,	-near * far / ( far - near ), 0.0f },
	};
	return Mtx4( temp );
}

Mtx4 Mtx4::create_simple_view_projection( float width, float height )
{
	float temp[4][4] {
		{ 2.0f / width, 0.0f,		   0.0f, 0.0f },
		{ 0.0f,			2.0f / height, 0.0f, 0.0f },
		{ 0.0f,			0.0f,		   1.0f, 0.0f },
		{ 0.0f,			0.0f,		   1.0f, 1.0f },
	};
	return Mtx4( temp );
}
