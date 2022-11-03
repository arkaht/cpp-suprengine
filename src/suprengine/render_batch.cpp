#include "render_batch.h"

#include "texture.h"
#include <suprengine/ecs/components/renderers/renderer.h>

using namespace suprengine;

RenderBatch::~RenderBatch()
{
	if ( sdl_renderer == nullptr ) return;

	TTF_Quit();

	SDL_DestroyRenderer( sdl_renderer );
}

bool RenderBatch::initialize( Window* _window )
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

void RenderBatch::begin_render()
{
	translation = Vec2::zero;

	SDL_SetRenderDrawColor( sdl_renderer, background_color.r, background_color.g, background_color.b, background_color.a );
	SDL_RenderClear( sdl_renderer );

	SDL_SetRenderDrawBlendMode( sdl_renderer, SDL_BLENDMODE_BLEND );
}

void RenderBatch::render()
{
	for ( auto renderer : renderers )
	{
		renderer->render();
	}
}

void RenderBatch::end_render()
{
	SDL_RenderPresent( sdl_renderer );
}

void RenderBatch::draw_rect( DrawType draw_type, const Rect& rect, const Color& color )
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

void RenderBatch::draw_texture( const Rect& src_rect, const Rect& dest_rect, const double rotation, const Vec2& origin, Texture* texture, const Color& color )
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
		rotation,
		&center,
		SDL_RendererFlip::SDL_FLIP_NONE
	);
}

void RenderBatch::add_renderer( Renderer* renderer )
{
	//  get priority order
	int order = renderer->get_priority_order();

	//  search order position
	auto itr = renderers.begin();
	for ( ; itr != renderers.end(); itr++ )
	{
		if ( order < (*itr)->get_priority_order() )
		{
			break;
		}
	}

	//  insert it
	renderers.insert( itr, renderer );
}

void RenderBatch::remove_renderer( Renderer* renderer )
{
	auto itr = std::find( renderers.begin(), renderers.end(), renderer );
	if ( itr == renderers.end() ) return;

	std::iter_swap( itr, renderers.end() - 1 );
	renderers.pop_back();
}
