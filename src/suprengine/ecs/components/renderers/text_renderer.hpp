
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

			//  delete old texture
			if ( texture != nullptr )
			{
				delete texture;
			}

			//  render text on surface
			SDL_Color white { 255, 255, 255, 255 };
			SDL_Surface* sdl_surface = TTF_RenderText_Blended( font->get_sdl_font(), text.c_str(), white );
			texture = render_batch->load_texture_from_surface( "TextRenderer::update_texture", sdl_surface );
			
			//  free surface
			SDL_FreeSurface( sdl_surface );

			//  setup
			dest.set_size( texture->get_size() );
			source = { 0.0f, 0.0f, dest.w, dest.h };
		}
	};
}