#include "render-batch.h"

#include <suprengine/ecs/components/renderers/renderer.h>

using namespace suprengine;

void RenderBatch::render()
{
	for ( auto renderer : renderers )
	{
		if ( !renderer->is_rendered ) continue;

		renderer->render();
	}
}

void RenderBatch::translate( const Vec2& pos )
{
	translation += pos;
}

Texture* RenderBatch::load_texture( rconst_str path, const TextureParams& params )
{
	SDL_Surface* surface = Texture::load_surface( path );
	if ( surface == nullptr ) return nullptr;

	Texture* texture = load_texture_from_surface( path, surface, params );
	SDL_FreeSurface( surface );

	return texture;
}

void RenderBatch::set_background_color( Color color )
{
	background_color = color;
}

void RenderBatch::add_renderer( Renderer* renderer )
{
	//  get priority order
	int order = renderer->get_priority_order();

	//  search order position
	auto itr = renderers.begin();
	for ( ; itr != renderers.end(); itr++ )
	{
		if ( order >= ( *itr )->get_priority_order() )
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

	renderers.erase( itr );  //  don't swap or you need to sort again!
}

