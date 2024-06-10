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
	camera_owner->transform->location = _test_pawn->transform->location;
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
	
	auto rabbit_data = _world->get_pawn_data( "rabbit" );
	auto grass_data = _world->get_pawn_data( "grass" );

	_test_pawn = _world->create_pawn( 
		rabbit_data, 
		Vec3 { _world->get_size() * 0.5f, 0.0f }
	);
	_world->create_pawn( grass_data, _world->find_random_tile_pos() );
	_world->create_pawn( grass_data, _world->find_random_tile_pos() );
	_world->create_pawn( grass_data, _world->find_random_tile_pos() );
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

		if ( _test_pawn.is_valid() )
		{
			_test_pawn->move_to( 
				_test_pawn->get_tile_pos() + dirs[random::generate( 0, 3 )]
			);
		}
	}
	if ( inputs->is_key_just_pressed( SDL_SCANCODE_F ) )
	{
		if ( _camera_controller->focus_target.lock() )
		{
			_camera_controller->focus_target.reset();
		}
		else if ( _test_pawn.is_valid() )
		{
			_camera_controller->focus_target = _test_pawn->transform;
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

	if ( _camera_controller->focus_target.is_valid() )
	{
		printf( "Target Location: %s\n", 
			_camera_controller->focus_target->location.to_string().c_str() );
		printf( "Hunger: %f\n", _test_pawn->hunger );
	}
}
