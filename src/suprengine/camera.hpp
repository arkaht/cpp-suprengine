#pragma once

#include "rect.h"

namespace suprengine
{
	class Camera
	{
	public:
		Rect viewport { 0, 0, 0, 0 };
		float zoom { 1.0f };

		Camera() {}

		void translate( Vec2 pos ) { viewport.add_pos( pos ); }

		void push( SDL_Renderer* sdl_renderer )
		{
			SDL_Rect sdl_viewport = ( Rect { viewport.x / zoom, viewport.y / zoom, viewport.w, viewport.h } ).to_sdl_rect();
			SDL_RenderSetViewport( sdl_renderer, &sdl_viewport );
			SDL_RenderSetScale( sdl_renderer, zoom, zoom );
		}

		void pop( SDL_Renderer* sdl_renderer )
		{
			SDL_RenderSetScale( sdl_renderer, 1.0f, 1.0f );
			SDL_RenderSetViewport( sdl_renderer, NULL );
		}

		Rect& get_viewport() { return viewport; }
	};
}