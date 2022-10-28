#include "render_batch.h"

#include "texture.h"
#include <suprengine/ecs/components/renderers/renderer.h>

using namespace suprengine;

RenderBatch::~RenderBatch()
{
	if ( _sdl_renderer == nullptr ) return;

	SDL_DestroyRenderer( _sdl_renderer );
}

bool RenderBatch::initialize( Window* _window )
{
	//  create renderer
	_sdl_renderer = SDL_CreateRenderer( _window->get_sdl_window(), -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
	if ( _sdl_renderer == nullptr )
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

	return true;
}

void RenderBatch::begin_render()
{
	SDL_SetRenderDrawColor( _sdl_renderer, 27, 27, 27, 255 );
	SDL_RenderClear( _sdl_renderer );
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
	SDL_RenderPresent( _sdl_renderer );
}

void RenderBatch::draw_rect( const Rect& rect, const Color& _color )
{
	SDL_SetRenderDrawColor( _sdl_renderer, _color.r, _color.g, _color.b, _color.a );

	auto sdl_rect = rect.to_sdl_rect();
	SDL_RenderFillRect( _sdl_renderer, &sdl_rect );
}

void RenderBatch::draw_texture( const Rect& src_rect, const Rect& dest_rect, const double rotation, const Vec2& origin, Texture* texture, const Color& color )
{
	SDL_Rect src = src_rect.to_sdl_rect(), dest = dest_rect.to_sdl_rect();
	SDL_Point center = ( origin * texture->get_size() ).to_sdl_point();

	//  apply origin
	if ( center.x > 0 || center.y > 0 )
	{
		dest.x -= center.x, dest.y -= center.y;
	}

	//  draw texture
	SDL_RenderCopyEx(
		_sdl_renderer,
		texture->get_sdl_texture(),
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
}