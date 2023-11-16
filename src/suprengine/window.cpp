#include "window.h"

using namespace suprengine;

Window::Window( const char* title ) 
	:	title( title ),
		size { DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT } {}
Window::Window( const char* title, const int width, const int height ) 
	:	title( title ), 
		size { (float)width, (float)height } {}

Window::~Window()
{
	if ( sdl_window == nullptr ) return;

	SDL_DestroyWindow( sdl_window );
}

bool Window::init()
{
	if ( SDL_Init( SDL_INIT_VIDEO ) != 0 )
	{
		Logger::error( "unable to initialize SDL" );
		return false;
	}

	sdl_window = SDL_CreateWindow( title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, size.x, size.y, SDL_WINDOW_OPENGL );
	if ( sdl_window == nullptr )
	{
		Logger::error( "unable to create SDL window" );
		return false;
	}

	return true;
}