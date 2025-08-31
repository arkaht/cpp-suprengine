#pragma once

#include <string>

namespace suprengine::string
{
	inline std::string bytes_to_str( const std::size_t bytes )
	{
		constexpr std::size_t KILO_IN_BYTES = 1024;
		constexpr std::size_t MEGA_IN_BYTES = 1024 * 1024;
		constexpr std::size_t GIGA_IN_BYTES = 1024 * 1024 * 1024;

		std::string unit = "XiB";
		float value = static_cast<float>( bytes );
		if ( bytes >= GIGA_IN_BYTES )
		{
			value /= static_cast<float>( GIGA_IN_BYTES );
			unit[0] = 'G';
		}
		else if ( bytes >= MEGA_IN_BYTES )
		{
			value /= static_cast<float>( MEGA_IN_BYTES );
			unit[0] = 'M';
		}
		else if ( bytes >= KILO_IN_BYTES )
		{
			value /= static_cast<float>( KILO_IN_BYTES );
			unit[0] = 'K';
		}
		else
		{
			unit = "B";
		}

		return std::format( "{:.2f} {}", value, unit );
	}
}