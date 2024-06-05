#include "render-batch.h"

#include <suprengine/texture.h>
#include <suprengine/game.h>
#include <suprengine/components/renderer.h>

using namespace suprengine;

RenderBatch::RenderBatch( Window* _window )
	: _window( _window )
{
	_engine = &Engine::instance();

	//  setup ambient light
	_ambient_light.color = Color::white;
	_ambient_light.direction = Vec3 { 0.3f, 1.0f, 0.3f }.normalized();
	_ambient_light.scale = 0.25f;

	//  update viewport on window size change
	_window->on_size_changed.listen( 
		"suprengine::render-batch", 
		[&]( const Vec2& size ) {
			on_window_resized( size );
		}
	);
}

void RenderBatch::_render_phase( const RenderPhase phase )
{
	if ( _renderers.find( phase ) == _renderers.end() ) return;

	auto& list = _renderers.at( phase );
	for ( auto renderer : list )
	{
		if ( !renderer->is_active ) continue;

		renderer->render( this );
	}
}

void RenderBatch::draw_texture( const Vec2& pos, const Vec2& scale, float rotation, const Vec2& origin, Texture* texture, const Color& color )
{
	Rect src_rect { Vec2::zero, texture->get_size() };
	Rect dest_rect { pos, texture->get_size() * scale };

	draw_texture( 
		src_rect, dest_rect, 
		rotation, origin, texture, color 
	);
}

Texture* RenderBatch::load_texture( rconst_str path, const TextureParams& params )
{
	SDL_Surface* surface = Texture::load_surface( path );
	if ( surface == nullptr ) return nullptr;

	Texture* texture = load_texture_from_surface( path, surface, params );
	SDL_FreeSurface( surface );

	return texture;
}

Texture* RenderBatch::load_texture_from_surface( rconst_str path, SDL_Surface* surface, const TextureParams& params )
{
	return new Texture( path, surface, params );
}

void RenderBatch::set_background_color( Color color )
{
	_background_color = color;
}

void RenderBatch::set_ambient_direction( const Vec3& direction )
{
	_ambient_light.direction = direction.normalized();
}

void RenderBatch::set_ambient_scale( float scale )
{
	_ambient_light.scale = scale;
}

void RenderBatch::set_ambient_color( Color color )
{
	_ambient_light.color = color;
}

void RenderBatch::add_renderer( Renderer* renderer )
{
	//  get priority order
	int order = renderer->get_priority_order();

	//  check phase creation
	RenderPhase phase = renderer->get_render_phase();
	if ( _renderers.find( phase ) == _renderers.end() )
		_renderers.insert( std::pair( phase, std::vector<Renderer*>() ) );

	//  search order position
	auto& list = _renderers.at( phase );
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
	if ( _renderers.find( phase ) == _renderers.end() ) return;

	//  find element in vector
	auto& list = _renderers.at( phase );
	auto itr = std::find( list.begin(), list.end(), renderer );
	if ( itr == list.end() ) return;

	list.erase( itr );  //  don't swap or you need to sort again!
}

