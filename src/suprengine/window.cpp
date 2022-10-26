#include "window.h"

using namespace suprengine;

Window::Window( const char* title ) : _title( title ) {}
Window::Window( const char* title, const int width, const int height ) : _title( title ), _width( width ), _height( height ) {}

bool Window::initialize()
{
	if ( SDL_Init( SDL_INIT_VIDEO ) != 0 )
	{
		Logger::error( "unable to initialize SDL" );
		return false;
	}

	_sdl_window = SDL_CreateWindow( _title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, _width, _height, 0 );
	if ( _sdl_window == nullptr )
	{
		Logger::error( "unable to create SDL window" );
		return false;
	}

	return true;
}

void Window::close()
{
	SDL_DestroyWindow( _sdl_window );
}