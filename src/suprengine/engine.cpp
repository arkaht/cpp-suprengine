#include "engine.h"

#include <suprengine/vec2.h>
#include <suprengine/assets.h>
#include <suprengine/entity.h>
#include <suprengine/components/transform.h>
#include <suprengine/components/collider.h>
#include <suprengine/scene.h>

#include <backends/imgui_impl_sdl2.h>
#include <implot.h>

#include <unordered_map>
#include <algorithm>

using namespace suprengine;

Engine::~Engine()
{
	clear_entities();

	//  Release game
	_game.reset( nullptr );

	//  Release scene
	_scene.reset( nullptr );

	//  Release managers
	_render_batch.reset( nullptr );
	_inputs.reset( nullptr );
	_physics.reset( nullptr );
	_window.reset( nullptr );

	//  Release assets
	Assets::release();

	//  Release ImGui
	ImGui_ImplSDL2_Shutdown();
	ImPlot::DestroyContext();
	ImGui::DestroyContext();

	//  Quit sdl
	SDL_Quit();
}

bool Engine::init( IGame* game )
{
	game->set_engine( this );
	_game = std::unique_ptr<IGame>( game );

	//  Get game infos
	auto infos = _game->get_infos();

	//  Init window
	_window = std::make_unique<Window>( 
		infos.title,
		infos.width,
		infos.height
	);
	if ( !_window->init() ) return false;

	//  Init render batch
	_render_batch = std::unique_ptr<RenderBatch>(
		game->create_render_batch( get_window() ) 
	);
	Assets::set_render_batch( get_render_batch() );
	if ( !_render_batch->init() ) return false;

	//  Setup window size on render batch
	_render_batch->on_window_resized( _window->get_size() );

	//  Init managers
	_inputs = std::make_unique<InputManager>();
	_physics = std::make_unique<Physics>();

	//  Init game
	_game->load_assets();
	_game->init();

	//  Init imgui
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImPlot::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
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
		//  Update::get_scaled_delta_time.
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
	//  Add to pending entities if currently updating...
	if ( _is_updating )
	{
		if ( std::find( _pending_entities.begin(), _pending_entities.end(), entity ) != _pending_entities.end() ) return;
		
		_pending_entities.push_back( entity );
	}
	//  Otherwise, add directly to active entities
	else
	{
		if ( std::find( _entities.begin(), _entities.end(), entity ) != _entities.end() ) return;

		_entities.push_back( entity );

		on_entity_added.invoke( entity );
	}
}

void Engine::remove_entity( SharedPtr<Entity> entity )
{
	//  Remove from actives
	auto itr = std::find( _entities.begin(), _entities.end(), entity );
	if ( itr != _entities.end() )
	{
		on_entity_removed.invoke( entity.get() );

		std::iter_swap( itr, _entities.end() - 1 );
		_entities.pop_back();
		return;
	}

	//  Remove from pendings
	itr = std::find( _pending_entities.begin(), _pending_entities.end(), entity );
	if ( itr != _pending_entities.end() )
	{
		std::iter_swap( itr, _pending_entities.end() - 1 );
		_pending_entities.pop_back();
	}
}

void Engine::clear_entities()
{
	//  Clear entities
	_pending_entities.clear();
	_entities.clear();
	_dead_entities.clear();

	//  Reset active camera
	camera = nullptr;
}

void Engine::add_timer( const Timer& timer )
{
	_timers.push_back( timer );
}

void Engine::process_input()
{
	//  Reset mouse inputs
	_inputs->mouse_delta = Vec2::zero;
	_inputs->mouse_wheel = Vec2::zero;

	ImGuiIO& imgui_io = ImGui::GetIO();

	//  Read window events
	SDL_Event event;
	while ( SDL_PollEvent( &event ) )
	{
		//  Send event to ImGui
		ImGui_ImplSDL2_ProcessEvent( &event );

		//  TODO?: Move this to the Window class
		//  TODO: Remove dependencies with our Event class
		switch ( event.type )
		{
		//  Store mouse delta for this frame
		case SDL_MOUSEMOTION:
			if ( imgui_io.WantCaptureMouse ) continue;

			_inputs->mouse_delta.x = (float)event.motion.xrel;
			_inputs->mouse_delta.y = (float)event.motion.yrel;
			break;
		//  Store mouse wheel for this frame
		case SDL_MOUSEWHEEL:
			if ( imgui_io.WantCaptureMouse ) continue;

			_inputs->mouse_wheel.x = (float)event.wheel.x;
			_inputs->mouse_wheel.y = (float)event.wheel.y;
			break;
		//  Quit game when closing window
		case SDL_QUIT:
			_is_running = false;
			break;
		}
	}
	
	//  Quit game when pressing a key
	if ( _inputs->is_key_just_pressed( SDL_SCANCODE_ESCAPE ) )
	{
		_is_running = false;
	}
	//  Toggle debug mode
	if ( _inputs->is_key_just_pressed( SDL_SCANCODE_COMMA ) )
	{
		is_debug = !is_debug;
	}
}

void Engine::update( float dt )
{
	_inputs->update();

	//  Add pending entities to active
	if ( !_pending_entities.empty() )
	{
		for ( auto& entity : _pending_entities )
		{
			add_entity( entity );
		}
		_pending_entities.clear();
	}

	//  Update scene
	_is_updating = true;
	if ( _scene )
	{
		_scene->update( dt );
	}

	//  Update entities
	for ( auto& entity : _entities )
	{
		if ( entity->state == EntityState::Active )
		{
			entity->update( dt );
		}

		//  Queue dead entity for later erase
		if ( entity->state == EntityState::Invalid )
		{
			_dead_entities.push_back( entity );
		}
	}
	_is_updating = false;

	//  Update colliders
	_physics->update();

	if ( !_timers.empty() )
	{
		//  TODO?: Merge both loops into one. Probably using iterators.
		//  Update timers
		for ( auto& timer : _timers )
		{
			if ( ( timer.time -= dt ) <= 0.0f )
			{
				timer.callback();
			}
		}

		//  Safely-erase expired timers
		auto itr = _timers.begin();
		while ( itr != _timers.end() )
		{
			if ( itr->time <= 0.0f )
			{
				//  Remove from vector
				itr = _timers.erase( itr );
			}
			else
			{
				itr++;
			}
		}
	}

	if ( !_dead_entities.empty() )
	{
		//  Delete dead entities
		for ( auto& entity : _dead_entities )
		{
			remove_entity( entity );
		}
		_dead_entities.clear();
	}
}

void Engine::render()
{
	//  Update ImGui
	_render_batch->begin_imgui_frame();
	on_imgui_update.invoke();

	//  Start rendering
	_render_batch->begin_render();

	//  Apply camera
	//  NOTE: This code doesn't do anything when using the OpenGL 
	//  render-batch. It was orignally here for supporting SDL and 
	//  2D.
	if ( camera != nullptr ) 
	{
		//  Apply transform
		_render_batch->scale( camera->zoom );
		_render_batch->translate( camera->viewport.get_pos() );

		//  Apply clipping
		if ( camera->clip_enabled )
		{
			_render_batch->clip( camera->clip );
		}
	}

	//  Render components
	_render_batch->render();

	//  Debug render entities & components
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

	//  End rendering
	_render_batch->end_render();
}
