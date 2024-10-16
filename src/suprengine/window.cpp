#include "window.h"

#include <suprengine/assert.hpp>

#include <SDL.h>

using namespace suprengine;

Window::Window( const std::string& title, int width, int height  ) 
	:	_title( title ), 
		_size { (float)width, (float)height }
{
	int sdl_init_code = SDL_Init( SDL_INIT_VIDEO );
	ASSERT( sdl_init_code > -1, SDL_GetError() );

	_sdl_window = SDL_CreateWindow( 
		_title.c_str(), 
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
		(int)_size.x, (int)_size.y,
		SDL_WINDOW_OPENGL
	);
	ASSERT( _sdl_window != nullptr, "Window couldn't be created!" );

	_current_size = _size;
}

Window::~Window()
{
	if ( _sdl_window == nullptr ) return;
	SDL_DestroyWindow( _sdl_window );
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
			SDL_SetWindowPosition( _sdl_window, 0, 0 );
			set_size( Vec2 { (float)display.w, (float)display.h }, false );
			break;
	}

	_mode = mode;
}

void Window::set_size( Vec2 size, bool is_new_size )
{
	const Vec2 old_size = _current_size;

	if ( is_new_size )
	{
		_size = size;
	}
	_current_size = size;

	//  resize window
	SDL_SetWindowSize( _sdl_window, (int)size.x, (int)size.y );

	//  broadcast event
	on_size_changed.invoke( size, old_size );
}
