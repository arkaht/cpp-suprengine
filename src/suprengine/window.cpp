#include "window.h"

using namespace suprengine;

Window::Window( const char* title ) : title( title ) {}
Window::Window( const char* title, const int width, const int height ) : title( title ), width( width ), height( height ) {}

Window::~Window()
{
	if ( sdl_window == nullptr ) return;

	SDL_DestroyWindow( sdl_window );
}

bool Window::initialize()
{
	if ( SDL_Init( SDL_INIT_VIDEO ) != 0 )
	{
		Logger::error( "unable to initialize SDL" );
		return false;
	}

	sdl_window = SDL_CreateWindow( title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL );
	if ( sdl_window == nullptr )
	{
		Logger::error( "unable to create SDL window" );
		return false;
	}

	return true;
}