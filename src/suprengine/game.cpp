#include "game.h"
#include "ecs/entity.h"
#include "ecs/components/transform.h"
#include "ecs/components/collider.h"
#include "scene.h"

#include <unordered_map>
#include <algorithm>

#include <suprengine/sdl/sdl-render-batch.h>
#include <suprengine/opengl/opengl-render-batch.h>

using namespace suprengine;

Game::~Game()
{
	clear_entities();

	//  release scene
	if ( scene != nullptr )
	{
		delete scene;
	}

	//  release assets
	Assets::release();

	//  free dependencies
	delete render_batch;
	delete window;
	delete inputs;
	delete physics;

	//  quit sdl
	SDL_Quit();
}

void Game::loop()
{
	while ( _is_running )
	{
		float dt = updater.compute_dt() / 1000.0f;

		process_input();
		update( dt );
		render();

		updater.accumulate_seconds( dt );
		updater.delay_time();
	}
}

void Game::set_scene( Scene* _scene )
{
	//  delete previous scene
	if ( scene != nullptr )
	{
		delete scene;
	}

	//  reset
	clear_entities();

	//  set scene
	scene = _scene;
	scene->init();
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

void Game::clear_entities()
{
	//  clear entities
	while ( !entities.empty() )
	{
		delete entities.back();
	}
}

void Game::add_timer( const Timer& timer )
{
	timers.push_back( timer );
}

void Game::process_input()
{
	inputs->mouse_delta = Vec2::zero;

	SDL_Event event;
	while ( SDL_PollEvent( &event ) )
	{
		switch ( event.type )
		{
		case SDL_MOUSEMOTION:
			inputs->mouse_delta = { (float) event.motion.xrel, (float) event.motion.yrel };
			break;
		//  quit game
		case SDL_QUIT:
			_is_running = false;
			break;
		}
	}
	
	//  quit
	if ( inputs->is_key_pressed( SDL_SCANCODE_ESCAPE ) )
	{
		_is_running = false;
	}
	//  toggle debug
	if ( inputs->is_key_pressed( SDL_SCANCODE_COMMA ) )
	{
		is_debug = !is_debug;
	}
}

void Game::update( float dt )
{
	inputs->update();

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
		if ( entity->get_state() == EntityState::ACTIVE )
			entity->update( dt );

		//  kill entity
		if ( entity->get_state() == EntityState::DEAD )
		{
			dead_entities.push_back( entity );
		}
	}
	_is_updating = false;

	//  update colliders
	physics->update();

	//  update timer
	if ( !timers.empty() )
	{
		for ( auto& timer : timers )
		{
			if ( ( timer.time -= dt ) <= 0.0f )
			{
				timer.callback();
			}
		}

		//  safely-erase expired timers
		auto itr = timers.begin();
		while ( itr != timers.end() )
		{
			if ( itr->time <= 0.0f )
			{
				//  remove from vector
				itr = timers.erase( itr );
			}
			else
			{
				itr++;
			}
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
	//  start rendering
	render_batch->begin_render();

	//  draw screen diagonals
	/*SDL_SetRenderDrawColor( sdl_renderer, 255, 255, 255, 255 );
	SDL_RenderDrawLine( sdl_renderer, 0, 0, _window->get_width() / camera.zoom, _window->get_height() / camera.zoom );
	SDL_RenderDrawLine( sdl_renderer, _window->get_width() / camera.zoom, 0, 0, _window->get_height() / camera.zoom );*/

	//  apply camera
	render_batch->scale( camera->zoom );
	render_batch->translate( camera->viewport.get_pos() );

	//  apply camera clipping
	if ( camera->clip_enabled )
	{
		render_batch->clip( camera->clip );
	}

	//  render components
	render_batch->render();

	//  debug render entities & components
	if ( is_debug )
	{
		for ( auto ent : entities )
		{
			ent->debug_render( render_batch );

			for ( auto comp : ent->components )
			{
				comp->debug_render( render_batch );
			}
		}
	}

	//  show rendering
	render_batch->end_render();
}
