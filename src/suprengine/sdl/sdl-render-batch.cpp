#include "sdl-render-batch.h"

#include <suprengine/texture.h>
#include <suprengine/ecs/components/renderers/renderer.h>

using namespace suprengine;

SDLRenderBatch::~SDLRenderBatch()
{
	if ( sdl_renderer == nullptr ) return;

	TTF_Quit();

	SDL_DestroyRenderer( sdl_renderer );
}

bool SDLRenderBatch::init()
{
	//  create renderer
	sdl_renderer = SDL_CreateRenderer( _window->get_sdl_window(), -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
	if ( sdl_renderer == nullptr )
	{
		Logger::error( "failed to create renderer" );
		return false;
	}

	//  initialize image library
	if ( IMG_Init( IMG_INIT_PNG ) == 0 )
	{
		Logger::error( "failed to initialize image library" );
		return false;
	}

	//  initialize ttf library
	if ( TTF_Init() == -1 )
	{
		Logger::error( "failed to initialize TTF library" );
		return false;
	}

	return true;
}

void SDLRenderBatch::begin_render()
{
	translation = Vec2::zero;

	SDL_SetRenderDrawColor( sdl_renderer, background_color.r, background_color.g, background_color.b, background_color.a );
	SDL_RenderClear( sdl_renderer );

	SDL_SetRenderDrawBlendMode( sdl_renderer, SDL_BLENDMODE_BLEND );
}

void SDLRenderBatch::end_render()
{
	SDL_RenderPresent( sdl_renderer );
}

void SDLRenderBatch::draw_rect( DrawType draw_type, const Rect& rect, const Color& color )
{
	SDL_SetRenderDrawColor( sdl_renderer, color.r, color.g, color.b, color.a );

	auto sdl_rect = rect.to_sdl_rect();

	//  apply translation
	sdl_rect.x += (int) translation.x, sdl_rect.y += (int) translation.y;

	switch ( draw_type )
	{
	case DrawType::FILL:
		SDL_RenderFillRect( sdl_renderer, &sdl_rect );
		break;
	case DrawType::LINE:
		SDL_RenderDrawRect( sdl_renderer, &sdl_rect );
		break;
	}
}

void SDLRenderBatch::draw_texture( const Rect& src_rect, const Rect& dest_rect, float rotation, const Vec2& origin, Texture* texture, const Color& color )
{
	SDL_Texture* sdl_texture = texture->get_sdl_texture();
	SDL_Rect src = src_rect.to_sdl_rect(), dest = dest_rect.to_sdl_rect();
	SDL_Point center = ( origin * src_rect.get_size() ).to_sdl_point();

	//  apply origin
	if ( center.x > 0 || center.y > 0 )
	{
		dest.x -= center.x, dest.y -= center.y;
	}

	//  apply translation
	dest.x += (int) translation.x, dest.y += (int) translation.y;

	//  modulate color
	SDL_SetTextureColorMod( sdl_texture, color.r, color.g, color.b );
	SDL_SetTextureBlendMode( sdl_texture, SDL_BLENDMODE_BLEND );
	SDL_SetTextureAlphaMod( sdl_texture, color.a );

	//  draw texture
	SDL_RenderCopyEx(
		sdl_renderer,
		sdl_texture,
		&src,
		&dest,
		rotation * math::RAD2DEG,
		&center,
		SDL_RendererFlip::SDL_FLIP_NONE
	);
}

void SDLRenderBatch::scale( float zoom )
{
	SDL_RenderSetScale( sdl_renderer, zoom, zoom );
}

void SDLRenderBatch::clip( const Rect& region )
{
	SDL_Rect clip_rect = region.to_sdl_rect();
	SDL_RenderSetClipRect( sdl_renderer, &clip_rect );
}

Texture* SDLRenderBatch::load_texture_from_surface( rconst_str path, SDL_Surface* surface, const TextureParams& params )
{
	return new SDLTexture( sdl_renderer, path, surface );
}
