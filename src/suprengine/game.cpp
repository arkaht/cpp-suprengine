#include "game.h"
#include "ecs/entity.h"
#include "ecs/components/transform2.hpp"

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
	_window = new Window( title, width, height );
	_render_batch = new RenderBatch();
	return _window->initialize() && _render_batch->initialize( _window );
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


void Game::process_input()
{
	SDL_Event event;
	while ( SDL_PollEvent( &event ) )
	{
		switch ( event.type )
		{
		case SDL_QUIT:
			_is_running = false;
			break;
		}
	}

	auto keyboard_state = SDL_GetKeyboardState( nullptr );
	//  quit on escape
	if ( keyboard_state[SDL_SCANCODE_ESCAPE] )
	{
		_is_running = false;
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

	//  delete dead entities
	if ( !dead_entities.empty() )
	{
		for ( auto entity : dead_entities )
		{
			delete entity;
		}
		dead_entities.clear();
	}

	/*player.x = player.x + 100.0f * dt;
	player.y = 100.0f + sinf( _timer.get_accumulated_seconds() * 3.0f ) * 50.0f;*/

	//player.transform->pos += Vec2 { 100.0f * dt, 20.0f * dt };
	//printf( "%f\n", dt);
}

void Game::render()
{
	_render_batch->begin_render();
	_render_batch->render();
	_render_batch->end_render();
}
