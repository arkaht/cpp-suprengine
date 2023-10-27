#include "logger.h"

#include <iostream>
#include <SDL_error.h>

using namespace suprengine;

void Logger::info( const std::string& message )
{
	std::cout << "INFO: " << message << "\n";
}

void Logger::error( const std::string& message )
{
	std::cout << "ERROR: " << message << "\n";
}

void Logger::sdl_error( LOG_CATEGORY category, const std::string& message )
{
	SDL_LogError( (int) category, "%s | SDL: %s", message.c_str(), SDL_GetError() );
}