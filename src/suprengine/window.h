#pragma once

#include <SDL.h>

#include "logger.h"

namespace suprengine
{
	constexpr int DEFAULT_WINDOW_WIDTH = 1280, DEFAULT_WINDOW_HEIGHT = 720;

	class Window
	{
	private:
		int width { DEFAULT_WINDOW_WIDTH }, height { DEFAULT_WINDOW_HEIGHT };
		const char* title { "My Suprengine Game" };

		SDL_Window* sdl_window { nullptr };
	public:
		Window( const char* title );
		Window( const char* title, const int width, const int height );
		Window( const Window& ) = delete;
		Window& operator=( const Window& ) = delete;
		~Window();

		bool initialize();

		SDL_Window* get_sdl_window() const { return sdl_window; }
		int get_width() const { return width; }
		int get_height() const { return height; }
	};
};

