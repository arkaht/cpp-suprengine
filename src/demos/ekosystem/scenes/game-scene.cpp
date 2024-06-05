#include "game-scene.h"

#include <suprengine/opengl/opengl-render-batch.h>
#include <suprengine/assets.h>
#include <suprengine/random.h>

using namespace eks;

GameScene::~GameScene()
{
	if ( _world != nullptr )
	{
		delete _world;
	}
}

void GameScene::init()
{
	auto& engine = Engine::instance();

	auto cube_model = Assets::get_model( MESH_CUBE );
	auto cylinder_model = Assets::get_model( MESH_CYLINDER );
	auto sphere_model = Assets::get_model( MESH_SPHERE );

	setup_world();

	//  setup camera
	auto camera_owner = engine.create_entity<Entity>();
	camera_owner->transform->location = _test_pawn.lock()->transform->location;
	camera_owner->transform->rotation = Quaternion( DegAngles { -45.0f, -135.0f, 0.0f } );
	_camera_controller = camera_owner->create_component<CameraController>( 
		50.0f, 
		Vec3 { 10.0f, 10.0f, 13.0f } 
	);
	auto camera = camera_owner->create_component<Camera>( CameraProjectionSettings {} );
	camera->activate();

	RenderBatch* render_batch = engine.get_render_batch();
	render_batch->set_ambient_direction( Vec3 { 0.0f, 0.0f, 1.0f } );
}

void GameScene::setup_world()
{
	_world = new World( Vec2 { 10.0f, 10.0f } );
	_test_pawn = _world->create_pawn( Vec3 { _world->get_size() * 0.5f, 0.0f } );
	_world->create_pawn( _world->find_random_tile_pos() );
	_world->create_pawn( _world->find_random_tile_pos() );
	_world->create_pawn( _world->find_random_tile_pos() );
}

void GameScene::update( float dt )
{
	auto engine = _game->get_engine();
	float time = engine->get_timer()->get_accumulated_seconds();

	auto inputs = engine->get_inputs();

	if ( inputs->is_key_just_pressed( SDL_SCANCODE_SPACE ) )
	{
		Vec3 dirs[] { 
			Vec3::forward, -Vec3::forward,
			Vec3::right, -Vec3::right 
		};

		if ( auto pawn = _test_pawn.lock() )
		{
			pawn->move_to( 
				pawn->get_tile_pos() + dirs[random::generate( 0, 3 )]
			);
		}
	}
	if ( inputs->is_key_just_pressed( SDL_SCANCODE_F ) )
	{
		if ( _camera_controller->focus_target.lock() )
		{
			_camera_controller->focus_target.reset();
		}
		else if ( auto pawn = _test_pawn.lock() )
		{
			_camera_controller->focus_target = pawn->transform;
		}
	}

	if ( inputs->is_key_just_pressed( SDL_SCANCODE_C ) )
	{
		if ( _world != nullptr )
		{
			delete _world;
			_world = nullptr;
			printf( "Delete world!\n" );
		}

		setup_world();
	}
	if ( inputs->is_key_just_pressed( SDL_SCANCODE_X ) )
	{
		if ( _world != nullptr )
		{
			delete _world;
			_world = nullptr;
			printf( "Delete world!\n" );
		}
	}

	if ( auto target = _camera_controller->focus_target.lock() )
	{
		printf( "Target Location: %s\n", target->location.to_string().c_str() );
	}
}
