#pragma once

#include <suprengine/math/vec2.h>

#include <suprengine/utils/event.h>

struct SDL_Window;

namespace suprengine
{
	enum class WindowMode
	{
		Windowed,
		Fullscreen,
		BorderlessFullscreen,
	};

	class Window
	{
	public:
		Window( const std::string& title, int width, int height );
		Window( const Window& ) = delete;
		Window& operator=( const Window& ) = delete;
		~Window();

		void set_mode( WindowMode mode );
		WindowMode get_mode() const { return _mode; }

		void set_size( Vec2 size, bool is_new_size = true );
		Vec2 get_size() const { return _current_size; }

		SDL_Window* get_sdl_window() const { return _sdl_window; }

	public:
		/*
		 * Called when window's size changed
		 * 
		 * Parameters:
		 * - const Vec2& new_size
		 * - const Vec2& old_size
		 */
		Event<const Vec2&, const Vec2&> on_size_changed;

	private:
		Vec2 _current_size = Vec2::zero;
		Vec2 _size = Vec2::zero;
		std::string _title {};

		WindowMode _mode = WindowMode::Windowed;

		SDL_Window* _sdl_window = nullptr;
	};
};

