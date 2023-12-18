#include "engine.h"

#include <suprengine/entity.h>
#include <suprengine/components/transform.h>
#include <suprengine/components/collider.h>
#include <suprengine/scene.h>

#include <unordered_map>
#include <algorithm>

using namespace suprengine;

Engine::~Engine()
{
	clear_entities();

	//  release game
	_game.reset( nullptr );

	//  release scene
	_scene.reset( nullptr );

	//  release managers
	_render_batch.reset( nullptr );
	_inputs.reset( nullptr );
	_physics.reset( nullptr );
	_window.reset( nullptr );

	//  release assets
	Assets::release();

	//  quit sdl
	SDL_Quit();
}

void Engine::loop()
{
	while ( _is_running )
	{
		float dt = _updater.compute_dt() / 1000.0f;

		process_input();
		update( dt );
		render();

		_updater.accumulate_seconds( dt );
		_updater.delay_time();
	}
}

void Engine::add_entity( Entity* entity )
{
	if ( _is_updating )
	{
		if ( std::find( _pending_entities.begin(), _pending_entities.end(), entity ) != _pending_entities.end() ) return;
		
		_pending_entities.push_back( entity );
	}
	else
	{
		if ( std::find( _entities.begin(), _entities.end(), entity ) != _entities.end() ) return;

		_entities.push_back( entity );
	}
}

void Engine::remove_entity( Entity* entity )
{
	//  remove from actives
	auto itr = std::find( _entities.begin(), _entities.end(), entity );
	if ( itr != _entities.end() )
	{
		std::iter_swap( itr, _entities.end() - 1 );
		_entities.pop_back();
		return;
	}

	//  remove from pendings
	itr = std::find( _pending_entities.begin(), _pending_entities.end(), entity );
	if ( itr != _pending_entities.end() )
	{
		std::iter_swap( itr, _pending_entities.end() - 1 );
		_pending_entities.pop_back();
	}
}

void Engine::clear_entities()
{
	//  clear entities
	while ( !_entities.empty() )
	{
		delete _entities.back();
	}
}

void Engine::add_timer( const Timer& timer )
{
	_timers.push_back( timer );
}

void Engine::process_input()
{
	_inputs->mouse_delta = Vec2::zero;

	//  read window events
	SDL_Event event;
	while ( SDL_PollEvent( &event ) )
	{
		//  TODO?: move this to the Window class
		//  TODO: remove dependencies with our Event class
		switch ( event.type )
		{
		case SDL_MOUSEMOTION:
			_inputs->mouse_delta = { (float) event.motion.xrel, (float) event.motion.yrel };
			break;
		//  quit game
		case SDL_QUIT:
			_is_running = false;
			break;
		}
	}
	
	//  quit
	if ( _inputs->is_key_just_pressed( SDL_SCANCODE_ESCAPE ) )
	{
		_is_running = false;
	}
	//  toggle debug
	if ( _inputs->is_key_just_pressed( SDL_SCANCODE_COMMA ) )
	{
		is_debug = !is_debug;
	}
}

void Engine::update( float dt )
{
	_inputs->update();

	//  add pending entities to active
	if ( !_pending_entities.empty() )
	{
		for ( auto entity : _pending_entities )
		{
			add_entity( entity );
		}
		_pending_entities.clear();
	}

	//  update scene & entities
	_is_updating = true;
	if ( _scene )
	{
		_scene->update( dt );
	}

	for ( auto entity : _entities )
	{
		if ( entity->state == EntityState::Active )
		{
			entity->update( dt );
		}

		//  kill entity
		if ( entity->state == EntityState::Invalid )
		{
			_dead_entities.push_back( entity );
		}
	}
	_is_updating = false;

	//  update colliders
	_physics->update();

	//  update timer
	if ( !_timers.empty() )
	{
		for ( auto& timer : _timers )
		{
			if ( ( timer.time -= dt ) <= 0.0f )
			{
				timer.callback();
			}
		}

		//  safely-erase expired timers
		auto itr = _timers.begin();
		while ( itr != _timers.end() )
		{
			if ( itr->time <= 0.0f )
			{
				//  remove from vector
				itr = _timers.erase( itr );
			}
			else
			{
				itr++;
			}
		}
	}

	//  delete dead entities
	if ( !_dead_entities.empty() )
	{
		for ( auto entity : _dead_entities )
		{
			delete entity;
		}
		_dead_entities.clear();
	}
}

void Engine::render()
{
	//  start rendering
	_render_batch->begin_render();

	//  apply camera
	if ( camera != nullptr ) 
	{
		//  transform
		_render_batch->scale( camera->zoom );
		_render_batch->translate( camera->viewport.get_pos() );

		//  clipping
		if ( camera->clip_enabled )
		{
			_render_batch->clip( camera->clip );
		}
	}

	//  render components
	_render_batch->render();

	//  debug render entities & components
	if ( is_debug )
	{
		auto _render_batch = get_render_batch();
		for ( auto entity : _entities )
		{
			entity->debug_render( _render_batch );

			for ( auto& component : entity->components )
			{
				component->debug_render( _render_batch );
			}
		}
	}

	//  show rendering
	_render_batch->end_render();
}
