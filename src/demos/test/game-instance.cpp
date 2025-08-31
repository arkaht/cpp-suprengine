#include "game-instance.h"

#include <scenes/game-scene.h>

#include <suprengine/core/assets.h>

using namespace test;

void GameInstance::load_assets()
{
    Assets::load_model( "cool-mesh", "assets/test/models/monkey.fbx" );
    SharedPtr<Model> cube = Assets::load_model( "cube", "assets/suprengine/models/cube.fbx" );
    cube->get_mesh( 0 )->add_texture( Assets::get_texture( TEXTURE_MEDIUM_GRID ) );
}

void GameInstance::init()
{
    Engine& engine = Engine::instance();
	InputManager* inputs = engine.get_inputs();

    //  setup inputs
    inputs->set_relative_mouse_mode( true );
	setup_input_actions( inputs );

    //  setup render batch
    auto render_batch = get_render_batch();
	render_batch->set_background_color( Color::from_0x( 0x000000 ) );

    //  load scene
	engine.create_scene<GameScene>();
}

void GameInstance::release()
{
}

GameInfos GameInstance::get_infos() const
{
	GameInfos infos {};
	infos.title	 = "Suprengine Test";
	infos.width	 = 1280;
	infos.height = 720;
	return infos;
}

void GameInstance::setup_input_actions( InputManager* inputs )
{
	InputAction<Vec2>* move_action = inputs->create_action<Vec2>( "Move" );
	move_action->assign_keys( Axis2D::Y, SDL_SCANCODE_S, SDL_SCANCODE_W );
	move_action->assign_keys( Axis2D::X, SDL_SCANCODE_A, SDL_SCANCODE_D );
	move_action->assign_gamepad_joystick( JoystickSide::Left );

	InputAction<Vec2>* look_action = inputs->create_action<Vec2>( "Look" );
	look_action->assign_mouse_delta();
	look_action->assign_gamepad_joystick( JoystickSide::Right, JoystickInputModifier::NegateY );

	InputAction<float>* vertical_action = inputs->create_action<float>( "Up/Down" );
	vertical_action->assign_keys( SDL_SCANCODE_Q, SDL_SCANCODE_E );
	vertical_action->assign_gamepad_buttons(
		GamepadButton::LeftTrigger,
		GamepadButton::RightTrigger
	);

	InputAction<bool>* sprint_action = inputs->create_action<bool>( "Sprint" );
	sprint_action->assign_key( SDL_SCANCODE_LSHIFT );
	sprint_action->assign_key( SDL_SCANCODE_RSHIFT );
	sprint_action->assign_mouse_button( MouseButton::Right );
	sprint_action->assign_gamepad_button( GamepadButton::LeftShoulder );
	sprint_action->assign_gamepad_button( GamepadButton::LeftThumb );
}
