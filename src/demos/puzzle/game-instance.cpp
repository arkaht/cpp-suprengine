#include "game-instance.h"

#include <scenes/game-scene.h>

using namespace puzzle;

void GameInstance::load_assets()
{
    //  shaders
	Assets::load_shader(
		"stylized",
		"assets/puzzle/shaders/stylized.vert",
		"assets/puzzle/shaders/stylized.frag"
	);

	//  textures
	Assets::load_texture( 
		"crosshair-line", 
		"assets/puzzle/sprites/crosshair-line.png" 
	);

	//  models
	Assets::load_model( 
		"spaceship", "assets/puzzle/models/spaceship2.fbx" );
	Assets::load_model( 
		"projectile", "assets/puzzle/models/projectile.fbx" );
	Assets::load_model( 
		"planet-ring", "assets/puzzle/models/planet-ring.fbx" );
	Assets::load_model( 
		"asteroid0", "assets/puzzle/models/asteroid0.fbx" );
	Assets::load_model( 
		"asteroid1", "assets/puzzle/models/asteroid1.fbx" );
}

void GameInstance::init()
{
    Engine& engine = Engine::instance();
    auto inputs = engine.get_inputs();

    //  setup inputs
    inputs->set_relative_mouse_mode( true );

    //  setup render batch
    auto render_batch = get_render_batch();
	render_batch->set_background_color( Color::from_0x( 0x00000000 ) );

    //  load scene
	engine.create_scene<GameScene>();
}

void GameInstance::release()
{
}

GameInfos GameInstance::get_infos() const
{
    GameInfos infos {};
    infos.title = "Spaceship X";
    infos.width = 1280;
	infos.height = 720;
    return infos;
}
