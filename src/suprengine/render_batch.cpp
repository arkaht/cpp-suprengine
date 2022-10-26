#include "render_batch.h"
#include "ecs/components/renderer.h"

using namespace suprengine;

bool RenderBatch::initialize( Window* _window )
{
	_sdl_renderer = SDL_CreateRenderer( _window->get_sdl_window(), -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
	if ( _sdl_renderer == nullptr )
	{
		Logger::error( "failed to create renderer" );
		return false;
	}

	return true;
}

void RenderBatch::close()
{
	SDL_DestroyRenderer( _sdl_renderer );
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

void RenderBatch::draw_rect( Rect& _rect, Color& _color )
{
	SDL_SetRenderDrawColor( _sdl_renderer, _color.r, _color.g, _color.b, _color.a );

	auto sdl_rect = _rect.to_sdl_rect();
	SDL_RenderFillRect( _sdl_renderer, &sdl_rect );
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
