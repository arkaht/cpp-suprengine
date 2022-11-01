#include "game.h"
#include "ecs/entity.h"
#include "ecs/components/transform2.hpp"
#include "ecs/components/collider.h"

using namespace suprengine;

Game::~Game()
{
	//  clear entities
	while ( !entities.empty() )
	{
		delete entities.back();
	}

	//  release assets
	Assets::release();

	//  release renderer
	delete _render_batch;

	//  release window
	delete _window;

	//  quit sdl
	SDL_Quit();
}

bool Game::initialize( const char* title, int width, int height )
{
	//  init window
	_window = new Window( title, width, height );
	if ( !_window->initialize() ) return false;

	//  init render batch
	_render_batch = new RenderBatch();
	if ( !_render_batch->initialize( _window ) ) return false;

	//  init assets
	Assets::set_render_batch( _render_batch );

	//  setup camera viewport
	camera.viewport.w = (float) width, camera.viewport.h = (float) height;

	return true;
}

void Game::loop()
{
	while ( _is_running )
	{
		float dt = _timer.compute_dt() / 1000.0f;

		process_input();
		update( dt );
		render();

		_timer.accumulate_seconds( dt );
		_timer.delay_time();
	}
}

void Game::add_entity( Entity* entity )
{
	if ( _is_updating )
	{
		if ( std::find( pending_entities.begin(), pending_entities.end(), entity ) != pending_entities.end() ) return;
		
		pending_entities.push_back( entity );
	}
	else
	{
		if ( std::find( entities.begin(), entities.end(), entity ) != entities.end() ) return;

		entities.push_back( entity );
	}
}

void Game::remove_entity( Entity* entity )
{
	//  remove from actives
	auto itr = std::find( entities.begin(), entities.end(), entity );
	if ( itr != entities.end() )
	{
		std::iter_swap( itr, entities.end() - 1 );
		entities.pop_back();
		return;
	}

	//  remove from pendings
	itr = std::find( pending_entities.begin(), pending_entities.end(), entity );
	if ( itr != pending_entities.end() )
	{
		std::iter_swap( itr, pending_entities.end() - 1 );
		pending_entities.pop_back();
	}
}

void Game::add_collider( Collider* collider )
{
	//  get priority order
	int order = collider->get_priority_order();

	//  search order position
	auto itr = colliders.begin();
	for ( ; itr != colliders.end(); itr++ )
	{
		if ( order < (*itr)->get_priority_order() )
		{
			break;
		}
	}

	colliders.push_back( collider );
}

void Game::remove_collider( Collider* collider )
{
	auto itr = std::find( colliders.begin(), colliders.end(), collider );
	if ( itr == colliders.end() ) return;

	std::iter_swap( itr, colliders.end() - 1 );
	colliders.pop_back();
}

bool Game::is_key_pressed( SDL_Scancode code )
{
	return get_key_state( code ) == KeyState::PRESSED;
}

bool Game::is_key_released( SDL_Scancode code )
{
	return get_key_state( code ) == KeyState::RELEASED;
}

bool Game::is_key_down( SDL_Scancode code )
{
	KeyState state = get_key_state( code );
	return state == KeyState::DOWN || state == KeyState::PRESSED;
}

bool Game::is_key_up( SDL_Scancode code )
{
	KeyState state = get_key_state( code );
	return state == KeyState::UP || state == KeyState::RELEASED;
}

KeyState Game::get_key_state( SDL_Scancode code )
{
	if ( keystates.find( code ) == keystates.end() ) return KeyState::UP;
	return keystates[code];
}

void Game::process_input()
{
	//  survey keystates
	//  NOTE: subject to key repeat :(
	if ( !survey_keys.empty() )
	{
		for ( auto itr = survey_keys.begin(); itr != survey_keys.end(); itr++ )
		{
			SDL_Scancode code = *itr;
			switch ( get_key_state( code ) )
			{
			case KeyState::PRESSED:
				keystates[code] = KeyState::DOWN;
				break;
			case KeyState::RELEASED:
				keystates[code] = KeyState::UP;
				break;
			}
		}
		survey_keys.clear();
	}

	SDL_Event event;
	while ( SDL_PollEvent( &event ) )
	{
		SDL_Scancode code;

		switch ( event.type )
		{
		case SDL_KEYUP:
			code = event.key.keysym.scancode;
			keystates[code] = KeyState::RELEASED;
			survey_keys.insert( code );
			break;
		case SDL_KEYDOWN:
			code = event.key.keysym.scancode;
			keystates[code] = KeyState::PRESSED;
			survey_keys.insert( code );
			break;
		//  quit game
		case SDL_QUIT:
			_is_running = false;
			break;
		}
	}
	
	//  quit
	if ( is_key_pressed( SDL_SCANCODE_ESCAPE ) )
	{
		_is_running = false;
	}
	//  toggle debug
	if ( is_key_pressed( SDL_SCANCODE_COMMA ) )
	{
		is_debug = !is_debug;
	}
}

void Game::update( float dt )
{
	//  add pending entities to active
	if ( !pending_entities.empty() )
	{
		for ( auto entity : pending_entities )
		{
			add_entity( entity );
		}
		pending_entities.clear();
	}

	//  update entities
	_is_updating = true;
	for ( auto entity : entities )
	{
		entity->update( dt );

		//  kill entity
		if ( entity->get_state() == EntityState::DEAD )
		{
			dead_entities.push_back( entity );
		}
	}
	_is_updating = false;

	//  update colliders
	for ( auto collider : colliders )
	{
		for ( auto other : colliders )
		{
			bool is_active = collider->intersects( other );

			collider->update_collision_with( other, is_active );
			other->update_collision_with( collider, is_active );
		}
	}

	//  delete dead entities
	if ( !dead_entities.empty() )
	{
		for ( auto entity : dead_entities )
		{
			delete entity;
		}
		dead_entities.clear();
	}
}

void Game::render()
{
	//  start renderering
	SDL_Renderer* sdl_renderer = _render_batch->get_sdl_renderer();
	_render_batch->begin_render();

	//  draw screen diagonals
	/*SDL_SetRenderDrawColor( sdl_renderer, 255, 255, 255, 255 );
	SDL_RenderDrawLine( sdl_renderer, 0, 0, _window->get_width() / camera.zoom, _window->get_height() / camera.zoom );
	SDL_RenderDrawLine( sdl_renderer, _window->get_width() / camera.zoom, 0, 0, _window->get_height() / camera.zoom );*/

	//  apply camera
	SDL_RenderSetScale( sdl_renderer, camera.zoom, camera.zoom );
	_render_batch->translate( camera.viewport.get_pos() );

	//  apply camera clipping
	if ( camera.clip_enabled )
	{
		SDL_Rect clip_rect = camera.clip.to_sdl_rect();
		SDL_RenderSetClipRect( sdl_renderer, &clip_rect );
	}

	//  render components
	_render_batch->render();

	//  debug render entities & components
	if ( is_debug )
	{
		for ( auto ent : entities )
		{
			ent->debug_render( _render_batch );

			for ( auto comp : ent->components )
			{
				comp->debug_render( _render_batch );
			}
		}
	}

	//  show rendering
	_render_batch->end_render();
}
