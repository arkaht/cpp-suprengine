#include "engine.h"

#include <suprengine/vec2.h>
#include <suprengine/assets.h>
#include <suprengine/entity.h>
#include <suprengine/components/transform.h>
#include <suprengine/components/collider.h>
#include <suprengine/scene.h>

#include <backends/imgui_impl_sdl2.h>

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

	//  release ImGui
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	//  quit sdl
	SDL_Quit();
}

bool Engine::init( IGame* game )
{
	game->set_engine( this );
	_game = std::unique_ptr<IGame>( game );

	//  get game infos
	auto infos = _game->get_infos();

	//  init window
	_window = std::make_unique<Window>( 
		infos.title, 
		infos.width, 
		infos.height
	);
	if ( !_window->init() ) return false;

	//  init render batch
	_render_batch = std::unique_ptr<RenderBatch>( 
		game->create_render_batch( get_window() ) 
	);
	Assets::set_render_batch( get_render_batch() );
	if ( !_render_batch->init() ) return false;

	//  setup window size on render batch
	_render_batch->on_window_resized( _window->get_size() );

	//  init managers
	_inputs = std::make_unique<InputManager>();
	_physics = std::make_unique<Physics>();

	//  init game
	_game->load_assets();
	_game->init();

	//  init imgui
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

	ImGui::StyleColorsDark();

	if ( !_render_batch->init_imgui() ) return false;

	return true;
}

void Engine::loop()
{
	while ( _is_running )
	{
		_updater.compute_delta_time();
		float dt = _updater.get_scaled_delta_time();

		//  Apply time scale modifiers
		//  NOTE: This won't affect result returned by 
		//        Update::get_scaled_delta_time.
		if ( is_game_paused )
		{
			dt = 0.0f;
		}

		process_input();
		update( dt );
		render();

		_updater.accumulate_seconds( dt );
		_updater.delay_time();
	}
}

void Engine::add_entity( SharedPtr<Entity> entity )
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

		on_entity_added.invoke( entity );
	}
}

void Engine::remove_entity( SharedPtr<Entity> entity )
{
	//  remove from actives
	auto itr = std::find( _entities.begin(), _entities.end(), entity );
	if ( itr != _entities.end() )
	{
		on_entity_removed.invoke( entity.get() );

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
	for ( auto& entity : _entities )
	{
		entity->kill();
	}

	//  clear entities
	_pending_entities.clear();
	_entities.clear();
	_dead_entities.clear();

	//  clear camera
	camera = nullptr;
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
		//  send event to ImGui
		ImGui_ImplSDL2_ProcessEvent( &event );

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
		for ( auto& entity : _pending_entities )
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

	for ( auto& entity : _entities )
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
		for ( auto& entity : _dead_entities )
		{
			remove_entity( entity );
		}
		_dead_entities.clear();
	}
}

void Engine::render()
{
	//  ImGui update
	_render_batch->begin_imgui_frame();
	on_imgui_update.invoke();

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
		for ( auto& entity : _entities )
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
