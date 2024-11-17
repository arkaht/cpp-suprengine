#include "game-scene.h"

#include <suprengine/opengl/opengl-render-batch.h>
#include <suprengine/assets.h>

#include <suprengine/components/colliders/sphere-collider.h>

#include <suprengine/vis-debug.h>

using namespace test;

void GameScene::init()
{
	auto& engine = Engine::instance();

	auto cube_model = Assets::get_model( MESH_CUBE );
	auto cylinder_model = Assets::get_model( MESH_CYLINDER );
	auto sphere_model = Assets::get_model( MESH_SPHERE );

	auto texture = Assets::get_texture( TEXTURE_LARGE_GRID );
	auto cool_model = Assets::get_model( "cool-mesh" );
	cool_model->get_mesh( 0 )->add_texture( texture );

	//  setup ground
	auto ground = engine.create_entity<Entity>();
	ground->transform->scale = Vec3 { 100.0f, 100.0f, 1.0f };
	ground->create_component<ModelRenderer>( cube_model, SHADER_LIT_MESH );
	ground_collider = ground->create_component<BoxCollider>( Box::ONE );

	auto player = engine.create_entity<Entity>();

	//  setup sphere
	//auto sphere = engine.create_entity<Entity>();
	//sphere->transform->location = Vec3 { 0.0f, 0.0f, 4.0f };
	//sphere->create_component<ModelRenderer>( sphere_model, SHADER_LIT_MESH, Color::blue );

	////  setup cube
	//auto cube = engine.create_entity<Entity>();
	//cube->transform->location = Vec3 { 0.0f, 4.0f, 4.0f };
	//cube->create_component<ModelRenderer>( cube_model, SHADER_LIT_MESH, Color::green );

	////  setup cylinder
	//auto cylinder = engine.create_entity<Entity>();
	//cylinder->transform->location = Vec3 { 4.0f, 0.0f, 4.0f };
	//cylinder->create_component<ModelRenderer>( cylinder_model, SHADER_LIT_MESH, Color::red );
	
	auto mesh = engine.create_entity<Entity>();
	mesh->transform->location = Vec3 { 0.0f, 0.0f, 5.0f };
	mesh->create_component<SphereCollider>( 2.0f );
	model_renderer = mesh->create_component<ModelRenderer>( cool_model, SHADER_LIT_MESH );

	//  setup camera
	auto camera_owner = engine.create_entity<CameraDemo>( player );
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
	auto inputs = engine->get_inputs();
	float time = engine->get_updater()->get_accumulated_seconds();

	VisDebug::add_line(
		Vec3 {
			0.0f,
			0.0f,
			math::sin( time * 2.0f ) * 10.0f
		},
		Vec3 {
			math::cos( time ) * 10.0f,
			math::sin( time ) * 10.0f,
			10.0f
		},
		Color::red
	);

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

	VisDebug::add_box(
		Vec3 { 0.0f, 0.0f, 1.0f + math::sin( time * 5.0f ) * 0.5f },
		Quaternion( DegAngles { time * 30.0f, time * 60.0f, time * 90.0f } ),
		Box::ONE,
		Color::white
	);

	if ( inputs->is_key_just_pressed( SDL_SCANCODE_F ) )
	{
		ground_collider->is_active = !ground_collider->is_active;
	}
}
