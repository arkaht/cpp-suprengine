#include "render-batch.h"

#include <suprengine/texture.h>
#include <suprengine/game.h>
#include <suprengine/components/renderer.h>

#include <suprengine/assets.h>

using namespace suprengine;

RenderBatch::RenderBatch( Window* _window )
	: _window( _window )
{
	//	Setup ambient light
	_ambient_light.color = Color::white;
	_ambient_light.direction = Vec3 { 0.3f, 1.0f, 0.3f }.normalized();
	_ambient_light.scale = 0.25f;

	//	Update viewport on window size change
	_window->on_size_changed.listen( 
		"suprengine::render-batch", 
		[&]( const Vec2& new_size, const Vec2& old_size ) {
			on_window_resized( new_size );
		}
	);
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

void RenderBatch::add_renderer( SharedPtr<Renderer> renderer )
{
	int order = renderer->get_priority_order();

	//  Ensure creation of the render phase map
	RenderPhase phase = renderer->get_render_phase();
	if ( _renderers.find( phase ) == _renderers.end() )
		_renderers.insert( std::pair( phase, std::vector<SharedPtr<Renderer>>() ) );

	//  Search order position
	auto& list = _renderers.at( phase );
	auto itr = list.begin();
	for ( ; itr != list.end(); itr++ )
		if ( order >= (*itr)->get_priority_order() )
			break;

	list.insert( itr, renderer );
}

void RenderBatch::remove_renderer( SharedPtr<Renderer> renderer )
{
	//  Ensure our render phase map exists
	RenderPhase phase = renderer->get_render_phase();
	if ( _renderers.find( phase ) == _renderers.end() ) return;

	//  Find element in vector
	auto& list = _renderers.at( phase );
	auto itr = std::find( list.begin(), list.end(), renderer );
	if ( itr == list.end() ) return;

	list.erase( itr );  //  Don't swap or you need to sort again!
}

void RenderBatch::init()
{
	Assets::set_render_batch( this );

	//	Update viewport size 
	on_window_resized( _window->get_size() );
}

void RenderBatch::draw_texture(
	const Vec2& pos,
	const Vec2& scale,
	float rotation,
	const Vec2& origin,
	SharedPtr<Texture> texture,
	const Color& color
)
{
	Rect src_rect { Vec2::zero, texture->get_size() };
	Rect dest_rect { pos, texture->get_size() * scale };

	draw_texture( 
		src_rect, dest_rect, 
		rotation, origin, texture, color 
	);
}

SharedPtr<Texture> RenderBatch::load_texture(
	rconst_str path,
	const TextureParams& params
)
{
	SDL_Surface* surface = Texture::load_surface( path );
	if ( surface == nullptr ) return nullptr;

	SharedPtr<Texture> texture = load_texture_from_surface( path, surface, params );
	SDL_FreeSurface( surface );

	return texture;
}

SharedPtr<Texture> RenderBatch::load_texture_from_surface(
	rconst_str path,
	SDL_Surface* surface,
	const TextureParams& params
)
{
	return std::make_shared<Texture>( path, surface, params );
}

int RenderBatch::get_renderers_count( const RenderPhase phase ) const
{
	auto itr = _renderers.find( phase );
	if ( itr == _renderers.end() ) return 0;

	return static_cast<int>( itr->second.size() );
}

void RenderBatch::_render_phase( const RenderPhase phase )
{
	if ( _renderers.find( phase ) == _renderers.end() ) return;

	std::vector<SharedPtr<Renderer>>& list = _renderers.at( phase );
	for ( SharedPtr<Renderer>& renderer : list )
	{
		if ( !renderer->is_active ) continue;
		renderer->render( this );
	}
}
