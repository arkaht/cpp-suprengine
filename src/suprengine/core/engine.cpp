#include "engine.h"

#include <suprengine/components/collider.h>
#include <suprengine/core/assets.h>
#include <suprengine/core/entity.h>
#include <suprengine/core/scene.h>

#include <suprengine/tools/profiler.h>
#include <suprengine/tools/vis-debug.h>

#include <backends/imgui_impl_sdl2.h>
#include <implot.h>

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
	if ( ImGui::GetCurrentContext() != nullptr )
	{
		ImGui_ImplSDL2_Shutdown();
		ImPlot::DestroyContext();
		ImGui::DestroyContext();
	}

	//  Quit sdl
	SDL_Quit();
}

#ifdef ENABLE_MEMORY_PROFILER
static void* custom_imgui_alloc( const std::size_t bytes, void* user_data )
{
	return MemoryProfiler::allocate( "ImGui", bytes );
}

static void custom_imgui_free( void* pointer, void* user_data )
{
	delete pointer;
}
#endif

bool Engine::init( IGame* game )
{
	PROFILE_SCOPE( "Engine::init" );

	//	Init SDL
	const int sdl_status = SDL_Init( SDL_INIT_VIDEO );
	ASSERT_MSG( sdl_status == 0, SDL_GetError() );

	//	Setup game
	game->set_engine( this );
	_game = std::unique_ptr<IGame>( game );

	//  Init window
	{
		const GameInfos infos = _game->get_infos();
		PROFILE_SCOPE( "Engine::init::Window" );

		_window = std::make_unique<Window>(
			infos.title,
			infos.width,
			infos.height,
			infos.window_mode
		);
	}

	//  Init render batch
	{
		PROFILE_SCOPE( "Engine::init::RenderBatch" );

		_render_batch = std::unique_ptr<RenderBatch>(
			game->create_render_batch( get_window() ) 
		);
		_render_batch->init();
	}

	//  Init managers
	_inputs = std::make_unique<InputManager>();
	_physics = std::make_unique<Physics>();

	//  Init game
	{
		PROFILE_SCOPE( "Engine::init::Game" );

		_game->load_assets();
		_game->init();
	}

	//  Init imgui
	{
		PROFILE_SCOPE( "Engine::init::ImGui" );

	#ifdef ENABLE_MEMORY_PROFILER
		//	Setup allocation functions to profile them
		ImGui::SetAllocatorFunctions( &custom_imgui_alloc, &custom_imgui_free, NULL );
	#endif

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImPlot::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

		ImGui::StyleColorsDark();

		if ( !_render_batch->init_imgui() ) return false;
	}

	return true;
}

void Engine::loop()
{
	while ( _is_running )
	{
		{
			PROFILE_SCOPE( "Engine::loop" );

			{
				PROFILE_SCOPE( "Engine::loop::no_delay" );

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
			}

			_updater.delay_time();
		}

		_profiler.update();
	}
}

void Engine::add_entity( const SharedPtr<Entity>& entity )
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

void Engine::remove_entity( const SharedPtr<Entity>& entity )
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

	//  Clear cameras
	_cameras.clear();
}

void Engine::add_timer( const Timer& timer )
{
	_timers.emplace_back( timer );
}

void Engine::add_camera( Camera* camera )
{
	ASSERT( std::find( _cameras.begin(), _cameras.end(), camera ) == _cameras.end() );
	_cameras.push_back( camera );
}

void Engine::remove_camera( const Camera* camera )
{
	auto itr = std::find( _cameras.begin(), _cameras.end(), camera );
	ASSERT( itr != _cameras.end() );
	_cameras.erase( itr );
}

bool Engine::is_running() const
{
	return _is_running;
}

void Engine::process_input()
{
	const bool can_run = _inputs->update();
	if ( !can_run )
	{
		_is_running = false;
		return;
	}
	
	//  Quit game when pressing a key
	if ( _inputs->is_key_just_pressed( SDL_SCANCODE_ESCAPE ) )
	{
		_is_running = false;
		return;
	}
#ifdef ENABLE_VISDEBUG
	//  Toggle debug mode
	if ( _inputs->is_key_just_pressed( SDL_SCANCODE_COMMA ) )
	{
		VisDebug::active_channels = VisDebug::active_channels == DebugChannel::None
			? DebugChannel::All
			: DebugChannel::None;
	}
#endif
}

void Engine::update( const float dt )
{
	PROFILE_SCOPE( "Engine::update" );

	//  Add pending entities to active
	if ( !_pending_entities.empty() )
	{
		PROFILE_SCOPE( "Engine::update::pending_entities" );

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
		PROFILE_SCOPE( "Engine::update::scene" );
		_scene->update( dt );
	}

	//  Update entities
	{
		PROFILE_SCOPE( "Engine::update::entities" );
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
	}
	_is_updating = false;

	//  Update colliders
	_physics->update();

	if ( !_timers.empty() )
	{
		PROFILE_SCOPE( "Engine::update::timers" );

		//  Update timers
		auto itr = _timers.begin();
		for ( itr; itr != _timers.end(); )
		{
			auto& timer = *itr;

			//	Increment timer until it exceeds the maximum time
			if ( ( timer.current_time += dt ) < timer.max_time )
			{
				itr++;
				continue;
			}
			
			//	Call timer's function
			timer.callback();

			//	Check if timer has repetitions
			//	NOTE: This condition allows to have infinite looping timers.
			if ( timer.repetitions > 0 )
			{
				//	Delete timer if repetitions reached zero
				timer.repetitions--;
				if ( timer.repetitions == 0 )
				{
					itr = _timers.erase( itr );
					continue;
				}
			}

			//	Reset timer's time
			//	NOTE: Substracting maximum time for more accurate results at higher delta time
			timer.current_time -= timer.max_time;

			itr++;
		}
	}

	if ( !_dead_entities.empty() )
	{
		PROFILE_SCOPE( "Engine::update::dead_entities" );

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
	PROFILE_SCOPE( "Engine::render" );

	//  Update ImGui
	{
		PROFILE_SCOPE( "Engine::render::UpdateImGui" );
		_render_batch->begin_imgui_frame();
		on_imgui_update.invoke();
	}

	//  Start rendering
	_render_batch->begin_render();

#ifdef ENABLE_VISDEBUG
	//  Debug render entities & components
	if ( VisDebug::is_channel_active( DebugChannel::Entity ) )
	{
		PROFILE_SCOPE( "Engine::render::debug" );

		RenderBatch* render_batch = get_render_batch();
		for ( const SharedPtr<Entity>& entity : _entities )
		{
			entity->debug_render( render_batch );

			for ( const SharedPtr<Component>& component : entity->components )
			{
				component->debug_render( render_batch );
			}
		}
	}
#endif

	//  Render scene
	for ( Camera* camera : _cameras )
	{
		if ( !camera->is_active() ) continue;
		_render_batch->render( camera );
	}

	//  End rendering
	_render_batch->end_render();
}
