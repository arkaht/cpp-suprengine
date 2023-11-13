#include "render-batch.h"

#include <suprengine/game.h>
#include <suprengine/components/renderer.h>

using namespace suprengine;

RenderBatch::RenderBatch( Window* _window )
	: _window( _window )
{
	game = &Game::instance();

	ambient_light.color = Color::white;
	ambient_light.direction = Vec3 { 0.3f, 1.0f, 0.3f }.normalized();
	ambient_light.scale = 0.25f;
}

void RenderBatch::render()
{
	render_phase( RenderPhase::SPRITE );
}

void RenderBatch::render_phase( const RenderPhase phase )
{
	if ( renderers.find( phase ) == renderers.end() ) return;

	auto& list = renderers.at( phase );
	for ( auto renderer : list )
	{
		if ( !renderer->should_render ) continue;

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

void RenderBatch::set_ambient_direction( const Vec3& direction )
{
	ambient_light.direction = direction.normalized();
}

void RenderBatch::set_ambient_scale( float scale )
{
	ambient_light.scale = scale;
}

void RenderBatch::set_ambient_color( Color color )
{
	ambient_light.color = color;
}

void RenderBatch::add_renderer( Renderer* renderer )
{
	//  get priority order
	int order = renderer->get_priority_order();

	//  check phase creation
	RenderPhase phase = renderer->get_render_phase();
	if ( renderers.find( phase ) == renderers.end() )
		renderers.insert( std::pair( phase, std::vector<Renderer*>() ) );

	//  search order position
	auto& list = renderers.at( phase );
	auto itr = list.begin();
	for ( ; itr != list.end(); itr++ )
		if ( order >= (*itr)->get_priority_order() )
			break;

	//  insert it
	list.insert( itr, renderer );
}

void RenderBatch::remove_renderer( Renderer* renderer )
{
	//  check phase
	RenderPhase phase = renderer->get_render_phase();
	if ( renderers.find( phase ) == renderers.end() ) return;

	//  find element in vector
	auto& list = renderers.at( phase );
	auto itr = std::find( list.begin(), list.end(), renderer );
	if ( itr == list.end() ) return;

	list.erase( itr );  //  don't swap or you need to sort again!
}

