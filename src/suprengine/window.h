#pragma once

#include <SDL.h>

#include "vec2.h"
#include "logger.h"

namespace suprengine
{
	constexpr int DEFAULT_WINDOW_WIDTH = 1280, DEFAULT_WINDOW_HEIGHT = 720;

	class Window
	{
	public:
		Window( const char* title );
		Window( const char* title, const int width, const int height );
		Window( const Window& ) = delete;
		Window& operator=( const Window& ) = delete;
		~Window();

		bool init();

		SDL_Window* get_sdl_window() const { return sdl_window; }
		Vec2 get_size() const { return size; }

	private:
		Vec2 size;
		const char* title;

		SDL_Window* sdl_window { nullptr };
	};
};

