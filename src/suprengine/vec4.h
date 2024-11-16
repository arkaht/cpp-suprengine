#pragma once

#include <string>

namespace suprengine
{
	struct Vec3;
	struct Mtx4;

	struct Vec4
	{
	public:
		float x = 0.0f;
		float y = 0.0f;
		float z = 0.0f;
		float w = 0.0f;

	public:
		Vec4() {}
		explicit Vec4( float value );
		explicit Vec4( float x, float y, float z, float w );
		explicit Vec4( const Vec3& vec, float w = 0.0f );

	public:
		std::string to_string() const;

	public:
		Vec4 operator*( const Mtx4& mtx );
		Vec4 operator/( float value );

		Vec4& operator*=( const Mtx4& mtx );
		Vec4& operator*=( float value );
		Vec4& operator/=( float value );
	};
}