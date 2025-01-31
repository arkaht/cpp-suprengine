#pragma once

#include "vec3.h"

namespace suprengine 
{
	struct DegAngles;

	//  3-axis Euler Angles in radians
	struct RadAngles
	{
	public:
		float p { 0.0f };  //  Pitch, in radians
		float y { 0.0f };  //  Yaw, in radians
		float r { 0.0f };  //  Roll, in radians

	public:
		explicit RadAngles();
		explicit RadAngles( float a );
		explicit RadAngles( float p, float y, float r );

		explicit RadAngles( const Vec3& vec );
		explicit RadAngles( const DegAngles& ang );

	public:
		float get_deg_pitch() const;
		float get_deg_yaw() const;
		float get_deg_roll() const;
	};

	//  3-axis Euler Angles in degrees
	struct DegAngles 
	{
	public:
		float p { 0.0f };  //  Pitch, in degrees
		float y { 0.0f };  //  Yaw, in degrees
		float r { 0.0f };  //  Roll, in degrees

	public:
		explicit DegAngles();
		explicit DegAngles( float a );
		explicit DegAngles( float p, float y, float r );

		explicit DegAngles( const Vec3& vec );
		explicit DegAngles( const RadAngles& ang );

	public:
		float get_radian_pitch() const;
		float get_radian_yaw() const;
		float get_radian_roll() const;
	};
}