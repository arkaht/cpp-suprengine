#pragma once

#include <string>

namespace suprengine
{
	class Logger
	{
	public:
		Logger() = delete;
		~Logger() = delete;
		Logger& operator=( const Logger& ) = delete;

		template<typename... Args>
		static void info( const std::string& message, Args... args )
		{
			printf( ( "[INFO] " + message + "\n" ).c_str(), args... );
		}
		template<typename... Args>
		static void error( const std::string& message, Args... args )
		{
			printf( ( "[ERROR] " + message + "\n" ).c_str(), args... );
		}
		template<typename... Args>
		static void critical( const std::string& message, Args... args )
		{
			printf( ( "[CRITICAL] " + message + "\n" ).c_str(), args... );
		}
	};
};
