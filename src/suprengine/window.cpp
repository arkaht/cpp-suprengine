#include "window.h"

using namespace suprengine;

Window::Window( const char* title ) 
	:	_title( title ),
		_size { DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT }
{}
Window::Window( const char* title, const int width, const int height ) 
	:	_title( title ), 
		_size { (float)width, (float)height }
{}

Window::~Window()
{
	if ( _sdl_window == nullptr ) return;
	SDL_DestroyWindow( _sdl_window );
}

bool Window::init()
{
	if ( SDL_Init( SDL_INIT_VIDEO ) != 0 )
	{
		Logger::error( "unable to initialize SDL" );
		return false;
	}

	_sdl_window = SDL_CreateWindow( 
		_title, 
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
		_size.x, _size.y,
		SDL_WINDOW_OPENGL
	);
	if ( _sdl_window == nullptr )
	{
		Logger::error( "unable to create SDL window" );
		return false;
	}

	_current_size = _size;
	
	return true;
}

void Window::set_mode( WindowMode mode )
{
	if ( mode == _mode ) return;

	SDL_DisplayMode display;
	SDL_GetCurrentDisplayMode( 0, &display );

	//  unset previous mode
	switch ( _mode )
	{
		case WindowMode::Fullscreen:
			SDL_SetWindowFullscreen( _sdl_window, 0 );
			break;
		case WindowMode::BorderlessFullscreen:
			SDL_SetWindowBordered( _sdl_window, SDL_TRUE );
			break;
	}

	//  set current mode
	switch ( mode )
	{
		case WindowMode::Windowed:
			set_size( _size, false );
			SDL_SetWindowPosition( _sdl_window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED );
			break;
		case WindowMode::Fullscreen:
			SDL_SetWindowFullscreen( _sdl_window, SDL_WINDOW_FULLSCREEN );
			break;
		case WindowMode::BorderlessFullscreen:
			SDL_SetWindowBordered( _sdl_window, SDL_FALSE );
			SDL_SetWindowPosition( _sdl_window, 0.0f, 0.0f );
			set_size( Vec2 { (float)display.w, (float)display.h }, false );
			break;
	}

	_mode = mode;
}

void Window::set_size( Vec2 size, bool is_new_size )
{
	if ( is_new_size )
	{
		_size = size;
	}
	_current_size = size;

	//  resize window
	SDL_SetWindowSize( _sdl_window, size.x, size.y );

	//  broadcast event
	on_size_changed.invoke( size );
}
