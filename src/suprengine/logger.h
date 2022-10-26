#pragma once

#include <string>
#include <SDL_log.h>

namespace suprengine
{
	enum class log_category
	{
		APPLICATION = SDL_LOG_CATEGORY_APPLICATION,
		ASSERT = SDL_LOG_CATEGORY_ASSERT,
		AUDIO = SDL_LOG_CATEGORY_AUDIO,
		ERROR = SDL_LOG_CATEGORY_ERROR,
		INPUT = SDL_LOG_CATEGORY_INPUT,
		RENDER = SDL_LOG_CATEGORY_RENDER,
		SYSTEM = SDL_LOG_CATEGORY_SYSTEM,
		TEST = SDL_LOG_CATEGORY_TEST,
		VIDEO = SDL_LOG_CATEGORY_VIDEO,
	};

	class Logger
	{
	public:
		Logger() = delete;
		~Logger() = delete;
		Logger& operator=( const Logger& ) = delete;

		static void info( const std::string& message );
		static void error( log_category category, const std::string& message );
		static void error( const std::string& message );
	};
};
