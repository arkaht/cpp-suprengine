#include "game-scene.h"

#include <suprengine/opengl/opengl-render-batch.h>
#include <suprengine/assets.h>

using namespace eks;

void GameScene::init()
{
	auto cube_model = Assets::get_model( MESH_CUBE );
	auto cylinder_model = Assets::get_model( MESH_CYLINDER );
	auto sphere_model = Assets::get_model( MESH_SPHERE );

	//  setup ground
	auto ground = new Entity();
	ground->transform->scale = Vec3 { 100.0f, 100.0f, 1.0f };
	ground->create_component<ModelRenderer>( cube_model, SHADER_LIT_MESH );
	ground->create_component<BoxCollider>( Box::HALF );

	//  setup cube
	_test_pawn = new Pawn( cube_model );
	_test_pawn->transform->location = Vec3 { 0.0f, 0.0f, 4.0f };

	//  setup camera
	auto camera_owner = new Entity();
	camera_owner->transform->location = Vec3 { 0.0f, 0.0f, 0.0f };
	camera_owner->transform->rotation = Quaternion( DegAngles { -45.0f, 45.0f, 0.0f } );
	_camera_controller = camera_owner->create_component<CameraController>( 
		50.0f, 
		Vec3 { -10.0f, -10.0f, 13.0f } 
	);
	auto camera = camera_owner->create_component<Camera>( CameraProjectionSettings {} );
	camera->activate();

	RenderBatch* render_batch = _game->get_engine()->get_render_batch();
	render_batch->set_ambient_direction( Vec3 { 0.0f, 0.0f, 1.0f } );
}

void GameScene::update( float dt )
{
	auto engine = _game->get_engine();
	float time = engine->get_timer()->get_accumulated_seconds();

	auto inputs = engine->get_inputs();

	if ( inputs->is_key_just_pressed( SDL_SCANCODE_SPACE ) )
	{
		_test_pawn->move_to( 
			_test_pawn->transform->location + Vec3::forward
		);
	}
	if ( inputs->is_key_just_pressed( SDL_SCANCODE_F ) )
	{
		if ( _camera_controller->focus_target.lock() )
		{
			_camera_controller->focus_target.reset();
		}
		else
		{
			_camera_controller->focus_target = _test_pawn->transform;
		}
	}
}
