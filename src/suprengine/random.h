#pragma once
#include <random>

#include "vec3.h"
#include "color.h"

namespace suprengine
{
	class random
	{
	public:
		static int generate( int min, int max )
		{
			static std::mt19937 _generator( (unsigned int)std::time( nullptr ) );
			std::uniform_int_distribution<> dist( min, max );
			return dist( _generator );
		}

		static float generate( float min, float max )
		{
			static std::mt19937 _generator( (unsigned int)std::time( nullptr ) );
			std::uniform_real_distribution<> dist( (double)min, (double)max );
			return (float)dist( _generator );
		}

		static Vec3 generate_location( 
			float min_x, float min_y, float min_z, 
			float max_x, float max_y, float max_z 
		)
		{
			return Vec3 {
				generate( min_x, max_x ),
				generate( min_y, max_y ),
				generate( min_z, max_z ),
			};
		}

		static Vec3 generate_direction()
		{
			//  https://math.stackexchange.com/a/44691
			float angle = generate( 0.0f, math::DOUBLE_PI );
			float z = generate( -1.0f, 1.0f );
			float m = math::sqrt( 1.0f - z * z );

			return Vec3 {
				m * math::cos( angle ),
				m * math::sin( angle ),
				z,	
			};
		}

		static Color generate_color( bool should_randomize_alpha = false )
		{
			return Color 
			{ 
				(unsigned char)random::generate( 0, 255 ),
				(unsigned char)random::generate( 0, 255 ),
				(unsigned char)random::generate( 0, 255 ),
				should_randomize_alpha 
					? (unsigned char)random::generate( 0, 255 )
					: (unsigned char)255
			};
		}

		static Vec3 generate_scale( float min, float max )
		{
			return Vec3( generate( min, max ) );
		}

		template <typename T>
		static const T& generate( const std::vector<T>& list )
		{
			return list[generate( 0, (int)list.size() - 1 )];
		}
	};
}