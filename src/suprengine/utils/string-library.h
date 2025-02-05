#include <string>

namespace suprengine::string
{
	inline std::string bytes_to_str( int bytes )
	{
		const int kilo_in_bytes = 1024;
		const int mega_in_bytes = 1024 * 1024;
		const int giga_in_bytes = 1024 * 1024 * 1024;

		std::string unit = "XiB";
		float value = static_cast<float>( bytes );
		if ( bytes >= giga_in_bytes )
		{
			value /= giga_in_bytes;
			unit[0] = 'G';
		}
		else if ( bytes >= mega_in_bytes )
		{
			value /= mega_in_bytes;
			unit[0] = 'M';
		}
		else if ( bytes >= kilo_in_bytes )
		{
			value /= kilo_in_bytes;
			unit[0] = 'K';
		}
		else
		{
			unit = "B";
		}

		return std::format( "{:.2f} {}", value, unit );
	}
}