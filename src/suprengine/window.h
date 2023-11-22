#pragma once

#include <SDL.h>

#include "vec2.h"
#include "logger.h"

#include "event.hpp"

namespace suprengine
{
	constexpr int DEFAULT_WINDOW_WIDTH = 1280, DEFAULT_WINDOW_HEIGHT = 720;

	enum class WindowMode
	{
		Windowed,
		Fullscreen,
		BorderlessFullscreen,
	};

	class Window
	{
	public:
		/*
		 * Called when window's size changed
		 * 
		 * Parameters:
		 * - const Vec2& size
		 */
		Event<const Vec2&> on_size_changed;

		Window( const char* title );
		Window( const char* title, const int width, const int height );
		Window( const Window& ) = delete;
		Window& operator=( const Window& ) = delete;
		~Window();

		bool init();

		void set_mode( WindowMode mode );
		WindowMode get_mode() { return _mode; }

		void set_size( Vec2 size, bool is_new_size = true );
		Vec2 get_size() const { return _current_size; }

		SDL_Window* get_sdl_window() const { return _sdl_window; }

	private:
		Vec2 _current_size;
		Vec2 _size;
		const char* _title;

		WindowMode _mode = WindowMode::Windowed;

		SDL_Window* _sdl_window { nullptr };
	};
};

