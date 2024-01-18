#include "game-scene.h"

using namespace puzzle;

void GameScene::init()
{
	//  setup planet
	auto planet = new Entity();
	planet->transform->location = Vec3 { 2000.0f, 500.0f, 30.0f };
	planet->transform->rotation = Quaternion( Vec3 { 0.0f, 12.0f, -6.0f } *math::DEG2RAD );
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
	spaceship1->transform->location = Vec3 { 0.0f, 0.0f, 0.0f };

	spaceship2 = new Spaceship();
	spaceship2->set_color( Color::from_0x( 0x9213f2FF ) );
	spaceship2->transform->location = Vec3 { 50.0f, 0.0f, 0.0f };

	//  possess it by player
	player_controller = new PlayerSpaceshipController();
	player_controller->possess( spaceship1 );

	ai_controller = new AISpaceshipController();
	ai_controller->possess( spaceship2 );
	ai_controller->target = spaceship1;
}

void GameScene::update( float dt )
{
	auto engine = _game->get_engine();
	auto inputs = engine->get_inputs();

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
}
