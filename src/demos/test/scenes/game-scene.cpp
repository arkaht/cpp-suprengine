#include "game-scene.h"

#include <suprengine/opengl/opengl-render-batch.h>

using namespace test;

void GameScene::init()
{
	Model* cube_model = Assets::get_model( "suprengine::cube" );
	Model* cylinder_model = Assets::get_model( "suprengine::cylinder" );
	Model* sphere_model = Assets::get_model( "suprengine::sphere" );

	//  setup ground
	auto ground = new Entity();
	ground->transform->scale = Vec3 { 100.0f, 100.0f, 1.0f };
	ground->create_component<ModelRenderer>( cube_model, SHADER_LIT_MESH );
	ground->create_component<BoxCollider>( Box::HALF );

	//  setup sphere
	auto sphere = new Entity();
	sphere->transform->location = Vec3 { 0.0f, 0.0f, 4.0f };
	sphere->create_component<ModelRenderer>( sphere_model, SHADER_LIT_MESH, Color::blue );

	//  setup cube
	auto cube = new Entity();
	cube->transform->location = Vec3 { 0.0f, 4.0f, 4.0f };
	cube->create_component<ModelRenderer>( cube_model, SHADER_LIT_MESH, Color::green );

	//  setup cylinder
	auto cylinder = new Entity();
	cylinder->transform->location = Vec3 { 4.0f, 0.0f, 4.0f };
	cylinder->create_component<ModelRenderer>( cylinder_model, SHADER_LIT_MESH, Color::red );

	//  setup camera
	auto camera_owner = new CameraDemo( sphere );
	camera_owner->transform->location = Vec3 { 5.0f, 3.0f, 7.0f };
	camera_owner->transform->look_at( cylinder->transform->location );
	camera_owner->create_component<Mover>();
	//camera_owner->create_component<MouseLooker>( 2.0f );
	auto camera = camera_owner->create_component<Camera>( CameraProjectionSettings {} );
	camera->activate();
}

void GameScene::update( float dt )
{
	auto engine = _game->get_engine();
	float time = engine->get_timer()->get_accumulated_seconds();

	RenderBatch* render_batch = engine->get_render_batch();
	render_batch->set_ambient_direction(
		Vec3 {
			cosf( time ),
			cosf( time ) * sinf( time ),
			sinf( time ),
		}
	);
}
