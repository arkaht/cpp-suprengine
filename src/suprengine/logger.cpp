#include "logger.h"

#include <SDL_error.h>

using namespace suprengine;

void Logger::info( const std::string& message )
{
	SDL_Log( message.c_str() );
}

void Logger::error( log_category category, const std::string& message )
{
	SDL_LogError( (int) category, "%s | SDL: %s", message.c_str(), SDL_GetError() );
}

void Logger::error( const std::string& message )
{
	Logger::error( log_category::APPLICATION, message );
}