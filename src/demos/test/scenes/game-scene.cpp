#include "game-scene.h"

#include <suprengine/opengl/opengl-render-batch.h>
#include <suprengine/assets.h>

using namespace test;

void GameScene::init()
{
	auto cube_model = Assets::get_model( MESH_CUBE );
	auto cylinder_model = Assets::get_model( MESH_CYLINDER );
	auto sphere_model = Assets::get_model( MESH_SPHERE );

	auto texture = Assets::get_texture( TEXTURE_LARGE_GRID );
	auto cool_model = Assets::get_model( "cool-mesh" );
	cool_model->get_mesh( 0 )->add_texture( texture );

	//  setup ground
	auto ground = new Entity();
	ground->transform->scale = Vec3 { 100.0f, 100.0f, 1.0f };
	ground->create_component<ModelRenderer>( cube_model, SHADER_LIT_MESH );
	ground->create_component<BoxCollider>( Box::HALF );

	auto player = new Entity();

	//  setup sphere
	//auto sphere = new Entity();
	//sphere->transform->location = Vec3 { 0.0f, 0.0f, 4.0f };
	//sphere->create_component<ModelRenderer>( sphere_model, SHADER_LIT_MESH, Color::blue );

	////  setup cube
	//auto cube = new Entity();
	//cube->transform->location = Vec3 { 0.0f, 4.0f, 4.0f };
	//cube->create_component<ModelRenderer>( cube_model, SHADER_LIT_MESH, Color::green );

	////  setup cylinder
	//auto cylinder = new Entity();
	//cylinder->transform->location = Vec3 { 4.0f, 0.0f, 4.0f };
	//cylinder->create_component<ModelRenderer>( cylinder_model, SHADER_LIT_MESH, Color::red );
	
	auto mesh = new Entity();
	mesh->transform->location = Vec3 { 0.0f, 0.0f, 5.0f };
	model_renderer = mesh->create_component<ModelRenderer>( cool_model, SHADER_LIT_MESH );

	//  setup camera
	auto camera_owner = new CameraDemo( player );
	camera_owner->transform->location = Vec3 { 5.0f, 3.0f, 7.0f };
	camera_owner->transform->look_at( mesh->transform->location );
	auto camera = camera_owner->create_component<Camera>( CameraProjectionSettings {} );
	camera->activate();

	RenderBatch* render_batch = _game->get_engine()->get_render_batch();
	render_batch->set_ambient_direction( Vec3 { 0.0f, 0.0f, 1.0f } );
}

void GameScene::update( float dt )
{
	auto engine = _game->get_engine();
	float time = engine->get_updater()->get_accumulated_seconds();

	RenderBatch* render_batch = engine->get_render_batch();
	/*render_batch->set_ambient_direction(
		Vec3 {
			cosf( time ),
			cosf( time ) * sinf( time ),
			sinf( time ),
		}
	);*/

	model_renderer->modulate = Color { 
		(unsigned char)( 222 + cosf( time ) * 33 ),
		(unsigned char)( 222 + sinf( time ) * 33 ),
		(unsigned char)( 222 + cosf( time ) * 33 ),
	};
	model_renderer->transform->set_location( 
		Vec3 { 0.0f, 0.0f, 5.0f + sinf( time ) * 0.5f } );
	model_renderer->transform->set_rotation( 
		model_renderer->transform->rotation + Quaternion( DegAngles { 0.0f, 35.0f * dt, 0.0f } ) );
}
