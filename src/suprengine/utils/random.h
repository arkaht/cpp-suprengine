#pragma once
#include <random>

#include <suprengine/math/vec3.h>
#include <suprengine/math/color.h>
#include <suprengine/math/quaternion.h>
#include <suprengine/math/box.h>

namespace suprengine
{
	class random
	{
	public:
		static int generate( const int min, const int max )
		{
			std::uniform_int_distribution dist( min, max );
			return dist( get_generator() );
		}

		static float generate( const float min, const float max )
		{
			std::uniform_real_distribution dist( min, max );
			return dist( get_generator() );
		}

		static float generate( const Vec2& bounds )
		{
			return generate( bounds.x, bounds.y );
		}

		template <typename T>
		static const T& generate( const std::vector<T>& list )
		{
			return list[generate( 0, static_cast<int>( list.size() ) - 1 )];
		}

		template <typename T>
		static const T& generate( const T* array, const int size )
		{
			return array[generate( 0, size - 1 )];
		}

		static bool generate_bool()
		{
			return generate( 0, 1 ) == 1;
		}

		static int generate_sign()
		{
			return generate_bool() ? 1 : -1;
		}

		static Vec3 generate_location(
			const float min_x, const float min_y, const float min_z,
			const float max_x, const float max_y, const float max_z
		)
		{
			return Vec3 {
				generate( min_x, max_x ),
				generate( min_y, max_y ),
				generate( min_z, max_z ),
			};
		}

		static Vec3 generate_location( const Box& box )
		{
			return generate_location(
				box.min.x, box.min.y, box.min.z,
				box.max.x, box.max.y, box.max.z
			);
		}

		static Vec3 generate_direction()
		{
			//  https://math.stackexchange.com/a/44691
			const float angle = generate( 0.0f, math::DOUBLE_PI );
			const float z = generate( -1.0f, 1.0f );
			const float m = math::sqrt( 1.0f - z * z );

			return Vec3 {
				m * math::cos( angle ),
				m * math::sin( angle ),
				z,
			};
		}

		static Quaternion generate_rotation()
		{
			return Quaternion::look_at( generate_direction(), Vec3::up );
		}

		static Color generate_color( const bool should_randomize_alpha = false )
		{
			return Color
			{
				static_cast<unsigned char>( generate( 0, 255 ) ),
				static_cast<unsigned char>( generate( 0, 255 ) ),
				static_cast<unsigned char>( generate( 0, 255 ) ),
				should_randomize_alpha
					? static_cast<unsigned char>( generate( 0, 255 ) )
					: static_cast<unsigned char>( 255 )
			};
		}

		static Vec3 generate_scale( const float min, const float max )
		{
			return Vec3( generate( min, max ) );
		}

		static void seed( const unsigned int seed )
		{
			auto& generator = get_generator();
			generator.seed( seed );
		}

		static std::mt19937& get_generator()
		{
			static std::mt19937 generator( static_cast<unsigned int>( std::time( nullptr ) ) );
			return generator;
		}
	};
}