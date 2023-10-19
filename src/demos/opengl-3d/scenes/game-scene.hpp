#pragma once

#include <suprengine/scene.h>
#include <suprengine/ecs/components/renderers/rect-renderer.hpp>
#include <suprengine/ecs/components/renderers/sprite-renderer.hpp>
#include <suprengine/ecs/components/renderers/text-renderer.hpp>
#include <suprengine/ecs/components/renderers/mesh-renderer.hpp>
#include <suprengine/ecs/components/box-collider.hpp>
#include <suprengine/ecs/components/sphere-collider.hpp>
#include <suprengine/ecs/components/spring-arm.hpp>
#include <suprengine/ecs/components/mover.hpp>
#include <suprengine/ecs/components/mouse-follower.hpp>
#include <suprengine/ecs/components/mouse-looker.hpp>
#include <suprengine/event.hpp>

#include "../camera-demo.hpp"

using namespace suprengine;

namespace demo_opengl3d
{
	class TimeRotator : public Component
	{
	public:
		Vec3 axis;
		float speed;

		TimeRotator( Entity* owner, const Vec3& axis, float speed = 1.0f )
			: axis( axis ), speed( speed ), Component( owner )
		{}

		void update( float dt ) override
		{
			Quaternion increment = Quaternion( axis, speed * dt );
			Quaternion rotation = Quaternion::concatenate( owner->transform->rotation, increment );
			transform->set_rotation( rotation );
		}
	};

	class JumpTester : public Component
	{
	public:
		Event<JumpTester*, bool> on_jump;

		JumpTester( Entity* owner )
			: Component( owner ) {}

		void update( float dt ) override
		{
			InputManager* inputs = owner->get_game()->get_inputs();
			if ( inputs->is_key_pressed( SDL_SCANCODE_SPACE ) ) 
			{
				transform->set_location( transform->location + Vec3::up * dt * 5.0f );

				on_jump.invoke( this, inputs->is_key_down( SDL_SCANCODE_LSHIFT ) );
			}
		}
	};

	class AchievementManager
	{
	private:
		int jump_count { 0 },
			unlock_jump_count { 100 };
	public:
		void handle_event_jump( JumpTester* jumper, bool is_lshifting )
		{
			if ( ++jump_count == unlock_jump_count )
			{
				//jumper->on_jump.unlisten( "achievement" );
				printf( "[Achievement Unlocked] 'No Gravity': 'You pressed the jump button %d times, maybe we will get physics one day?'", unlock_jump_count );
			}
			else if ( jump_count == 50 )
			{
				jumper->on_jump.unlisten( "test_on_jump" );
			}
			printf( "%d/%d\n", jump_count, unlock_jump_count );
		}
	};

	void test_on_jump( JumpTester* jumper, bool is_lshifting )
	{
		printf( "test_on_jump()\n" );
	}

	class GameScene : public Scene
	{
	public:
		GameScene() {}

		void init() override 
		{
			printf( "\n" );

			game->get_inputs()->set_relative_mouse_mode( true );
			game->get_render_batch()->set_background_color( Color::from_0x( 0x252627FF ) );

			Mesh* mesh_cube = Assets::get_mesh( Assets::PRIMITIVE_CUBE_PATH, false );

			auto sphere = new Entity();
			sphere->transform->location = Vec3 { 0.0f, 5.0f, -10.0f };
			sphere->transform->scale = Vec3 { 0.25f, 0.25f, 0.25f };
			new MeshRenderer( sphere, Assets::get_mesh( Assets::PRIMITIVE_SPHERE_PATH, false ) );
			new SphereCollider( sphere, 12.5f );
			//new TimeRotator( sphere, Vec3::one, 0.5f );

			auto ground = new Entity();
			ground->transform->scale = Vec3 { 100.0f, 1.0f, 100.0f };
			new MeshRenderer( ground, mesh_cube );
			new BoxCollider( ground,
				Box {
					Vec3 { -0.5f, -0.5f, -0.5f },
					Vec3 { 0.5f, 0.5f, 0.5f }
				}
			);

			auto cube = new Entity();
			cube->transform->location = Vec3 { 0.0f, 5.0f, 10.0f };
			new Mover( cube );
			new MouseLooker( cube, 1.0f );
			new MeshRenderer( cube, mesh_cube );
			new BoxCollider( cube,
				Box {
					Vec3 { -0.5f, -0.5f, -0.5f },
					Vec3 { 0.5f, 0.5f, 0.5f }
				}
			);

			//  observer pattern usage (but function-based)
			AchievementManager* achievement_manager = new AchievementManager();
			JumpTester* jumper = new JumpTester( cube );
			jumper->on_jump.listen( "lambda",
				[]( JumpTester* jumper, bool is_lshifting )
				{
					printf( "jump %p %d\n", jumper, is_lshifting );
				} 
			);
			jumper->on_jump.listen( "achievement",
				std::bind( &AchievementManager::handle_event_jump, achievement_manager, std::placeholders::_1, std::placeholders::_2 )
			);
			jumper->on_jump.listen( "test_on_jump", test_on_jump );


			auto text = new Entity();
			text->transform->location = Vec3 { 0.0f, 0.0f, 5.0f };
			new TextRenderer( text, Assets::get_font( "PressStart2P.ttf" ), "3D?" );
			//new TimeRotator( text, Vec3::one );

			auto camera_owner = new CameraDemo( cube );
			auto camera = new Camera( camera_owner, 77.7f );
			camera->activate();
		}
	};
}