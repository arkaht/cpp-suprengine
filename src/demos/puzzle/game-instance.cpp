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
	Assets::load_texture( 
		"kill-icon", 
		"assets/puzzle/sprites/kill-icon.png" 
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
	Assets::load_model( 
		"explosion0", "assets/puzzle/models/explosion0.fbx" );
	Assets::load_model( 
		"explosion1", "assets/puzzle/models/explosion1.fbx" );
	Assets::load_model( 
		"explosion2", "assets/puzzle/models/explosion2.fbx" );

	//  curves
	Assets::load_curve(
		"explosion-transform-scale", "assets/puzzle/curves/explosion-transform-scale.cvx" );
	Assets::load_curve(
		"explosion-outline-scale", "assets/puzzle/curves/explosion-outline-scale.cvx" );
	Assets::load_curve(
		"explosion-outline-color", "assets/puzzle/curves/explosion-outline-color.cvx" );
	Assets::load_curve(
		"explosion-inner-color", "assets/puzzle/curves/explosion-inner-color.cvx" );
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
