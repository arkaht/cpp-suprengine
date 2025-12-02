#include "game-scene.h"

#include <suprengine/core/assets.h>

#include <suprengine/rendering/opengl/opengl-render-batch.h>

#include <suprengine/components/colliders/sphere-collider.h>

#include <suprengine/tools/vis-debug.h>

#include <suprengine/utils/random.h>

#include "tests/unit-test-event.h"

#include <GL/glew.h>

using namespace test;

void on_imgui_update()
{
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar
			| ImGuiWindowFlags_NoSavedSettings; //  Disable saved settings to avoid weird sizes caused by window modes

	ImGui::SetNextWindowSize( ImVec2 { 400, 600 }, ImGuiCond_Appearing );

	bool is_visible = true;
	if ( !ImGui::Begin( "Debug Menu", &is_visible, window_flags ) )
	{
		ImGui::End();
		return;
	}

	//  Show ImGui demo window
	static bool show_imgui_demo = false;
	if ( show_imgui_demo ) ImGui::ShowDemoWindow( &show_imgui_demo );

	//  Populate menu bar
	if ( ImGui::BeginMenuBar() )
	{
		if ( ImGui::BeginMenu( "Engine" ) )
		{
			ImGui::EndMenu();
		}
		if ( ImGui::BeginMenu( "ImGui" ) )
		{
			ImGui::MenuItem( "ImGui Demo", nullptr, &show_imgui_demo );
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}

	if ( ImGui::CollapsingHeader( "Input", ImGuiTreeNodeFlags_DefaultOpen ) )
	{
		Engine& engine = Engine::instance();
		engine.get_inputs()->populate_imgui();
	}

	ImGui::End();
}

void GameScene::init()
{
	UnitTestEvent().run();

	auto& engine = Engine::instance();
	engine.on_imgui_update.listen( &on_imgui_update );

	InputManager* inputs = engine.get_inputs();
	InputAction<Vec2>* move_action = inputs->get_action<Vec2>( "Move" );
	InputAction<Vec2>* look_action = inputs->get_action<Vec2>( "Look" );
	InputAction<float>* vertical_action = inputs->get_action<float>( "Up/Down" );
	InputAction<bool>* sprint_action = inputs->get_action<bool>( "Sprint" );

	auto cube_model = Assets::get_model( "cube" );
	auto cylinder_model = Assets::get_model( MESH_CYLINDER );
	auto sphere_model = Assets::get_model( MESH_SPHERE );

	auto texture = Assets::get_texture( TEXTURE_LARGE_GRID );
	auto cool_model = Assets::get_model( "cool-mesh" );
	cool_model->get_mesh( 0 )->add_texture( texture );

	//  setup ground
	/*auto ground = engine.create_entity<Entity>();
	ground->transform->scale = Vec3 { 100.0f, 100.0f, 1.0f };
	ground->create_component<ModelRenderer>( cube_model, SHADER_LIT_MESH );
	ground_collider = ground->create_component<BoxCollider>( Box::one );*/

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
	
	/*auto mesh = engine.create_entity<Entity>();
	mesh->transform->location = Vec3 { 0.0f, 0.0f, 5.0f };
	mesh->create_component<SphereCollider>( 2.0f );
	model_renderer = mesh->create_component<ModelRenderer>( cool_model, SHADER_LIT_MESH );*/

	//  setup camera
	auto camera_owner = engine.create_entity<CameraDemo>( player, move_action, look_action, sprint_action, vertical_action );
	camera_owner->transform->location = Vec3 { 5.0f, 3.0f, 7.0f };
	//camera_owner->transform->look_at( mesh->transform->location );
	auto camera = camera_owner->create_component<Camera>( CameraProjectionSettings {} );
	camera->set_active();

	RenderBatch* render_batch = _game->get_engine()->get_render_batch();
	render_batch->set_ambient_direction( -Vec3::up );
	render_batch->set_ambient_scale( 0.1f );

	VisDebug::active_channels = DebugChannel::Entity;
}

void GameScene::update( float dt )
{
	auto engine = _game->get_engine();
	auto inputs = engine->get_inputs();
	float time = engine->get_updater()->get_accumulated_seconds();

	if ( inputs->is_key_just_pressed( PhysicalKey::F1 ) )
	{
		inputs->set_relative_mouse_mode( !inputs->is_relative_mouse_mode_enabled() );
	}
	
	Vec3 pos { 0.0f, 0.0f, 5.0f };
	/*VisDebug::add_box(
		pos + Vec3 { 0.0f, 0.0f, math::sin( time * 5.0f ) * 0.5f },
		Quaternion( DegAngles { time * 30.0f, time * 60.0f, time * 90.0f } ),
		Box::one * math::abs( math::cos( time * 1.0f ) * 1.5f ),
		Color::white
	);
	pos.x += 5.0f;

	VisDebug::add_line(
		pos + Vec3 {
			0.0f,
			0.0f,
			math::sin( time * 2.0f ) * 2.0f
		},
		pos + Vec3 {
			math::cos( time ) * 10.0f,
			math::sin( time ) * 10.0f,
			2.0f
		},
		Color::red
	);
	pos.x += 5.0f;*/

	static float spawn_time = 0.0f;
	if ( ( spawn_time -= dt ) < 0.0f )
	{
		for ( int i = 0; i < 10; i++ )
		{
			const float lifetime = random::generate( 0.5f, 1.5f );
			const Vec3 bounds = Vec3 { 15.0f, 5.0f, 5.0f };
			const Vec3 location = pos + random::generate_location( Box { -bounds, bounds } );
			switch ( random::generate( 0, 2 ) )
			{
				case 0:
					VisDebug::add_sphere(
						location,
						random::generate( 1.0f, 3.0f ),
						random::generate_color(),
						lifetime
					);
					break;
				case 1:
					VisDebug::add_box( 
						location,
						random::generate_rotation(),
						Box::half * random::generate( 0.1f, 2.0f ),
						random::generate_color(),
						lifetime
					);
					break;
				case 2:
					VisDebug::add_line(
						location,
						location + random::generate_location( Box::one * random::generate( 1.0f, 10.0f ) ),
						random::generate_color(),
						lifetime
					);
					break;
			}
		}
		spawn_time = 0.3f;
	}

	RenderBatch* render_batch = engine->get_render_batch();
	/*render_batch->set_ambient_direction(
		Vec3 {
			cosf( time ),
			cosf( time ) * sinf( time ),
			sinf( time ),
		}
	);*/

	if ( model_renderer )
	{
		model_renderer->modulate = Color { 
			(unsigned char)( 222 + cosf( time ) * 33 ),
			(unsigned char)( 222 + sinf( time ) * 33 ),
			(unsigned char)( 222 + cosf( time ) * 33 ),
		};
		model_renderer->transform->set_location( 
			Vec3 {
				0.0f,
				0.0f,
				5.0f + sinf( time ) * 0.5f
			}
		);
		model_renderer->transform->set_rotation( 
			model_renderer->transform->rotation + Quaternion( DegAngles { 0.0f, 35.0f * dt, 0.0f } )
		);
	}

	if ( inputs->is_key_just_pressed( PhysicalKey::F ) )
	{
		ground_collider->is_active = !ground_collider->is_active;
	}
}
