#pragma once

#include "sprite_renderer.hpp"

#include <suprengine/font.h>

namespace suprengine
{
	class TextRenderer : public SpriteRenderer
	{
	public:
		Font* font { nullptr };
		std::string text { "" };

		TextRenderer( Entity* owner, Font* font, const std::string& text, int priority_order = 0 )
			: font( font ), SpriteRenderer( owner, nullptr, priority_order ) 
		{
			set_text( text );
		}
	
		void set_text( const std::string& _text )
		{
			text = _text;
			update_texture();
		}

		void update_texture()
		{
			if ( font == nullptr ) return;

			//  render text on surface
			//  TODO: abstract it
			SDL_Surface* sdl_surface = TTF_RenderText_Solid(font->get_sdl_font(), text.c_str(), modulate.to_sdl_color());
			SDL_Texture* sdl_texture = render_batch->load_texture_from_surface( sdl_surface );
			
			//  delete old texture
			if ( texture != nullptr )
			{
				delete texture;
			}

			//  create another one
			texture = new Texture( "", sdl_texture, { (float) sdl_surface->w, (float) sdl_surface->h } );
			dest.w = texture->get_size().x, dest.h = texture->get_size().y;
			source = { 0.0f, 0.0f, dest.w, dest.h };

			//  free surface
			SDL_FreeSurface( sdl_surface );
		}
	};
}