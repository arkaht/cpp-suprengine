#include "game-scene.h"

#include <suprengine/opengl/opengl-render-batch.h>
#include <suprengine/assets.h>
#include <suprengine/random.h>

using namespace eks;

GameScene::GameScene()
{
	auto& engine = Engine::instance();
	engine.on_imgui_update.listen( "eks_menu", 
		std::bind( &GameScene::_show_imgui_menu, this )
	);
}

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
	
	auto hare_data  = _world->get_pawn_data( "hare" );
	auto grass_data = _world->get_pawn_data( "grass" );
	auto wolf_data  = _world->get_pawn_data( "wolf" );

	_world->create_pawn( hare_data, _world->find_random_tile_pos() );
	_world->create_pawn( hare_data, _world->find_random_tile_pos() );
	_world->create_pawn( hare_data, _world->find_random_tile_pos() );
	_test_pawn = _world->create_pawn( wolf_data, _world->find_random_tile_pos() );
	_world->create_pawn( grass_data, _world->find_random_tile_pos() );
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
	}
}

void GameScene::_show_imgui_menu()
{
	auto& engine = Engine::instance();

	//  Setup default position and size of the window
	const ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos( { viewport->WorkPos.x + 850, viewport->WorkPos.y + 20 }, ImGuiCond_FirstUseEver );
	ImGui::SetNextWindowSize( { 400, 680 }, ImGuiCond_FirstUseEver );

	ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar;

	if ( !ImGui::Begin( "Ekosystem Debug Menu", nullptr, window_flags ) )
	{
		ImGui::End();
		return;
	}

	static bool show_demo_menu = false;
	if ( show_demo_menu ) ImGui::ShowDemoWindow( &show_demo_menu );

	//  Populate menu bar
	if ( ImGui::BeginMenuBar() )
	{
		if ( ImGui::BeginMenu( "Development" ) )
		{
			ImGui::MenuItem( "Show Demo Menu", NULL, &show_demo_menu );
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}

	//  Populate Engine header
	if ( ImGui::CollapsingHeader( "Engine", ImGuiTreeNodeFlags_DefaultOpen ) )
	{
		//  Pause & time scale
		ImGui::Checkbox( "Pause Game", &engine.is_game_paused );
		ImGui::SliderFloat( "Time Scale", &engine.time_scale, 0.001f, 32.0f );

		std::array<float, 9> numbers {
			0.25f, 0.5f, 0.75f, 
			1.0f, 
			2.0f, 4.0f, 8.0f, 16.0f, 32.0f 
		};
		for ( int i = 0; i < numbers.size(); i++ )
		{
			float scale = numbers[i];

			//  Create string buffer
			char buffer[6];
			if ( scale < 1.0f )
			{
				sprintf_s( buffer, "x%.02f", scale );
			}
			else
			{
				sprintf_s( buffer, "x%d", (int)scale );
			}

			if ( ImGui::Button( buffer ) )
			{
				engine.time_scale = scale;
			}

			if ( i < numbers.size() - 1 )
			{
				ImGui::SameLine();
			}
		}
	}
	if ( ImGui::CollapsingHeader( "Ecosystem", ImGuiTreeNodeFlags_DefaultOpen ) )
	{
		static int selected_pawn_id = 0;
		auto& pawns = _world->get_pawns();

		//  Populate pawns
		ImGui::SeparatorText( "Pawns" );
		if ( ImGui::BeginTable( "eks_pawns", 4 ) )
		{
			ImGui::TableSetupColumn( "UID", ImGuiTableColumnFlags_WidthFixed );
			ImGui::TableSetupColumn( "Name", ImGuiTableColumnFlags_WidthFixed );
			ImGui::TableSetupColumn( "Action", ImGuiTableColumnFlags_WidthFixed );
			ImGui::TableSetupColumn( "Hunger", ImGuiTableColumnFlags_WidthFixed );
			//ImGui::TableSetupScrollFreeze( 4, 5 );
			ImGui::TableHeadersRow();

			ImGuiSelectableFlags selectable_flags = 
				ImGuiSelectableFlags_SpanAllColumns 
			  | ImGuiSelectableFlags_AllowOverlap;
			for ( int i = 0; i < pawns.size(); i++ )
			{
				ImGui::PushID( i );
				ImGui::TableNextRow( ImGuiTableRowFlags_None );

				auto& pawn = pawns[i];
				if ( pawn.is_valid() )
				{
					//  Column 0: ID
					ImGui::TableSetColumnIndex( 0 );
					char buffer[5];
					sprintf_s( buffer, "%04d", pawn->get_unique_id() );
					if ( ImGui::Selectable( 
						buffer, 
						selected_pawn_id == i,
						selectable_flags
					) )
					{
						selected_pawn_id = i;
					}

					//  Column 1: Name
					ImGui::TableSetColumnIndex( 1 );
					ImGui::Text( pawn->get_name().c_str() );

					//  Column 2: Action
					ImGui::TableSetColumnIndex( 2 );
					if ( ImGui::SmallButton( "Focus" ) )
					{
						_camera_controller->focus_target = pawn->transform;
					}
					ImGui::SameLine();
					if ( ImGui::SmallButton( "Kill" ) )
					{
						pawn->kill();
					}

					//  Column 3: Hunger
					ImGui::TableSetColumnIndex( 3 );
					float hunger_ratio = math::clamp( pawn->hunger / pawn->data->max_hunger, 0.0f, 1.0f );
					ImGui::ProgressBar( hunger_ratio, { 0.0f, 0.0f } );
					//ImGui::Text( "%d%%", (int)( hunger_ratio * 100 ) );
				
				}
				else
				{
					ImGui::TableSetColumnIndex( 0 );
					ImGui::Selectable( "????", selected_pawn_id == i, selectable_flags );
					
					ImGui::TableSetColumnIndex( 1 );
					ImGui::TextUnformatted( "N/A" );
					
					ImGui::TableSetColumnIndex( 2 );
					ImGui::TextUnformatted( "N/A" );
					
					ImGui::TableSetColumnIndex( 3 );
					ImGui::TextUnformatted( "0%" );
				}

				ImGui::PopID();
			}

			ImGui::EndTable();
		}

		static int selected_pawn_data_id = 0;
		const char* items[3] {
			"hare",
			"wolf",
			"grass",
		};
		ImGui::Combo( "Pawn Data", &selected_pawn_data_id, items, 3 );
		if ( ImGui::Button( "Create Pawn" ) )
		{
			auto data = _world->get_pawn_data( items[selected_pawn_data_id] );
			_world->create_pawn( data, _world->find_random_tile_pos() );
		}

		ImGui::Spacing();

		ImGui::SeparatorText( "Selected Pawn" );
		if ( selected_pawn_id >= 0 && selected_pawn_id < pawns.size() )
		{
			auto& pawn = pawns[selected_pawn_id];
			if ( pawn.is_valid() )
			{
				ImGui::Text( ( "Name: " + pawn->get_name() ).c_str() );

				//  Compute hunger
				float hunger = pawn->hunger;
				float max_hunger = pawn->data->max_hunger;
				float hunger_ratio = math::clamp( 
					hunger / max_hunger,
					0.0f,
					1.0f
				);

				//  Create progress text
				char buffer[32];
				sprintf_s( 
					buffer, "%.03f/%.03f (%d%%)",
					hunger, max_hunger, (int)( hunger_ratio * 100 )
				);

				//  Display hunger
				ImGui::ProgressBar( hunger_ratio, { 0.0f, 0.0f }, buffer );
				ImGui::SameLine( 0.0f, ImGui::GetStyle().ItemInnerSpacing.x );
				ImGui::Text( "Hunger" );
			}
			else
			{
				ImGui::Text( "Name: None" );
			}
		}
	}

	ImGui::End();
}
