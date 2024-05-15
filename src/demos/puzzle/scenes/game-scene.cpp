#include "game-scene.h"

#include "game-instance.h"

#include "entities/explosion-effect.h"
#include "components/player-hud.h"

using namespace puzzle;

GameScene::GameScene( GameInstance* game_instance )
	: _game_instance( game_instance )
{
}

void GameScene::init()
{
	random::seed( _seed );

	//  setup planet
	auto planet = new Entity();
	planet->transform->location = Vec3 { 2000.0f, 500.0f, 30.0f };
	planet->transform->rotation = Quaternion( DegAngles { -6.0f, 0.0f, 12.0f } );
	planet->transform->scale = Vec3( 10.0f );
	planet->create_component<StylizedModelRenderer>(
		Assets::get_model( "planet-ring" ),
		Color::from_0x( 0x1c6cF0FF )
	);

	//  spawn asteroids
	const int ASTEROID_COUNT = 32;
	const Vec3 ASTEROIDS_LOCATION = Vec3 { 500.0f, 100.0f, 50.0f };

	for ( int i = 0; i < ASTEROID_COUNT; i++ )
	{
		auto asteroid = new Asteroid();
		asteroid->transform->location = ASTEROIDS_LOCATION + random::generate_location(
			-300.0f, -300.0f, -300.0f,
			300.0f, 300.0f, 300.0f
		);
		asteroid->transform->rotation = Quaternion::look_at( random::generate_direction(), Vec3::up );
		asteroid->transform->scale = random::generate_scale( 4.0f, 30.0f );
		asteroid->linear_direction = Vec3::right * 2.0f * random::generate( 0.8f, 1.2f );
		asteroid->update_collision_to_transform();
	}

	//  spawn spaceships
	spaceship1 = new Spaceship();
	spaceship1->set_color( Color::from_0x( 0xf2cd13FF ) );
	spaceship1->transform->location = _player_location;
	spaceship1->transform->rotation = _player_rotation;

	spaceship2 = new Spaceship();
	spaceship2->set_color( Color::from_0x( 0x9213f2FF ) );
	spaceship2->transform->location = Vec3 { 50.0f, 0.0f, 0.0f };

	//  possess it by player
	player_controller = new PlayerSpaceshipController();
	player_controller->possess( spaceship1 );
	
	ai_controller = new AISpaceshipController();
	ai_controller->possess( spaceship2 );
	ai_controller->target = spaceship1;

	//  instantiate temporary camera
	auto projection_settings = player_controller->get_camera()->projection_settings;
	projection_settings.fov = 50.0f;
	projection_settings.znear = 10.0f;

	auto camera_owner = new Entity();
	_temporary_camera = camera_owner->create_component<Camera>( projection_settings );
	_temporary_camera_model = camera_owner->create_component<ModelRenderer>( Assets::get_model( MESH_ARROW ) );

	generate_ai_spaceships( 10 );
}

void GameScene::update( float dt )
{
	auto engine = _game->get_engine();
	auto inputs = engine->get_inputs();

	//  switch spaceship possession
	if ( inputs->is_key_just_pressed( SDL_SCANCODE_1 ) )
	{
		player_controller->possess( spaceship1 );
		ai_controller->possess( spaceship2 );
	}
	if ( inputs->is_key_just_pressed( SDL_SCANCODE_2 ) )
	{
		player_controller->possess( spaceship2 );
	}

	if ( ( spawn_time -= dt ) <= 0.0f )
	{
		auto explosion = new ExplosionEffect( 15.0f, random::generate_color() );
		explosion->transform->location = Vec3 { 0.0f, 100.0f, 0.0f };
		spawn_time += 2.5f;
	}
	
	//  window mode toggle
	if ( inputs->is_key_just_pressed( SDL_SCANCODE_F1 ) )
	{
		auto window = engine->get_window();
		if ( window->get_mode() != WindowMode::Windowed )
		{
			window->set_mode( WindowMode::Windowed );
		}
		else
		{
			window->set_mode( WindowMode::BorderlessFullscreen );
		}
	}
	if ( inputs->is_key_just_pressed( SDL_SCANCODE_F2 ) )
	{
		auto renderer = _game_instance->get_render_batch();
		renderer->set_samples( renderer->get_samples() == 0 ? 8 : 0 );
	}
	if ( inputs->is_key_just_pressed( SDL_SCANCODE_F3 ) )
	{
		auto spaceship = player_controller->get_ship();
		_player_location = spaceship->transform->location;
		_player_rotation = spaceship->transform->rotation;

		engine->clear_entities();
		init();
	}
	if ( inputs->is_key_just_pressed( SDL_SCANCODE_F4 ) )
	{
		if ( spaceship1->get_color().a == 0 )
		{
			spaceship1->set_color( Color::from_0x( 0xf2cd13FF ) );
		}
		else
		{
			spaceship1->set_color( Color { 0, 0, 0, 0 } );
		}
	}
	//  F5: place temporary camera
	if ( inputs->is_key_just_pressed( SDL_SCANCODE_F5 ) )
	{
		auto spaceship = player_controller->get_ship();
		_temporary_camera->transform->set_location( spaceship->transform->location );
		_temporary_camera->transform->set_rotation( spaceship->transform->rotation );
		printf( "Temporary camera has been moved!\n" );
	}
	//  F6: switch from spaceship camera to temporary camera
	if ( inputs->is_key_just_pressed( SDL_SCANCODE_F6 ) )
	{
		if ( _temporary_camera->is_active() )
		{
			player_controller->get_camera()->activate();
			player_controller->get_hud()->should_render = true;
			_temporary_camera_model->should_render = true;
			printf( "Switch camera to spaceship camera!\n" );
		}
		else
		{
			_temporary_camera->activate();
			player_controller->get_hud()->should_render = false;
			_temporary_camera_model->should_render = false;
			printf( "Switch camera to temporary camera!\n" );
		}
	}
	//  F7: lock player inputs
	if ( inputs->is_key_just_pressed( SDL_SCANCODE_F7 ) )
	{
		player_controller->is_inputs_enabled = !player_controller->is_inputs_enabled;
	}
}

void GameScene::generate_ai_spaceships( int count )
{
	std::vector<Spaceship*> potential_targets;
	std::vector<AISpaceshipController*> controllers;
	for ( int i = 0; i < count; i++ )
	{
		auto spaceship = new Spaceship();
		spaceship->set_color( random::generate_color() );
		spaceship->transform->location = Vec3 
		{ 
			100.0f + i * 50.0f,
			0.0f,
			0.0f
		};
		potential_targets.push_back( spaceship );

		auto controller = new AISpaceshipController();
		controller->possess( spaceship );
		controllers.push_back( controller );
	}

	auto rd = std::random_device {};
	auto rng = std::default_random_engine { rd() };
	std::shuffle( potential_targets.begin(), potential_targets.end(), rng );

	for ( auto controller : controllers )
	{
		controller->target = potential_targets[potential_targets.size() - 1];
		potential_targets.pop_back();
	}
}
