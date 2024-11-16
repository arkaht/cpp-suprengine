#include "mtx4.h"

using namespace suprengine;

static float m4Ident[4][4] =
{
	{ 1.0f, 0.0f, 0.0f, 0.0f },
	{ 0.0f, 1.0f, 0.0f, 0.0f },
	{ 0.0f, 0.0f, 1.0f, 0.0f },
	{ 0.0f, 0.0f, 0.0f, 1.0f }
};

const Mtx4 Mtx4::identity( m4Ident );

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
