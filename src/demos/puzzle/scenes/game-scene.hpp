#pragma once

#include <suprengine/scene.h>
#include <suprengine/components/colliders/box-collider.hpp>
#include <suprengine/components/mover.hpp>
#include <suprengine/components/mouse-looker.hpp>

#include <components/stylized-model-renderer.hpp>
#include <components/projectile.hpp>

namespace puzzle 
{
	class GameScene : public Scene
	{
	public:
		void init() override 
		{
			//_game->get_inputs()->set_relative_mouse_mode( true );

			RenderBatch* render_batch = _game->get_render_batch();
			render_batch->set_background_color( Color::from_0x( 0x00000000 ) );

			auto shader = Assets::load_shader(
				"stylized",
				"assets/puzzle/shaders/stylized.vert",
				"assets/puzzle/shaders/stylized.frag"
			);
			auto spaceship_model = Assets::load_model( 
				"spaceship", 
				"assets/puzzle/models/spaceship2.fbx",
				""
			);
			Assets::load_model( "projectile", "assets/puzzle/models/projectile.fbx", "" );

			Model* cube_model = Assets::get_model( "suprengine::cube" );
			Model* cylinder_model = Assets::get_model( "suprengine::cylinder" );
			Model* sphere_model = Assets::get_model( "suprengine::sphere" );
			
			//  setup ground
			/*auto ground = new Entity();
			ground->transform->scale = Vec3 { 100.0f, 1.0f, 100.0f };
			ground->create_component<ModelRenderer>( cube_model, "simple-mesh" );
			ground->create_component<BoxCollider>( Box::HALF );*/

			//  setup sphere
			auto planet = new Entity();
			planet->transform->location = Vec3 { 100.0f, 50.0f, 75.0f };
			planet->transform->scale = Vec3( 50.0f );
			planet->create_component<StylizedModelRenderer>( sphere_model, Color::blue );
			
			//  setup cube
			spaceship = new Entity();
			spaceship->transform->location = Vec3 { 0.0f, 0.0f, 0.0f };
			//spaceship->transform->scale = Vec3( 0.01f );
			spaceship->transform->rotation = SPACESHIP_ROTATION_OFFSET;
			spaceship->create_component<StylizedModelRenderer>( spaceship_model, Color::green );

			//  setup cylinder
			/*auto cylinder = new Entity();
			cylinder->transform->location = Vec3 { 3.0f, 2.0f, 0.0f };
			cylinder->create_component<StylizedModelRenderer>( cylinder_model, Color::red );*/
			
			//  setup camera
			auto camera_owner = new Entity();
			camera_owner->transform->location = -Vec3::forward * 7.0f;
			//camera_owner->transform->look_at( spaceship->transform->location );
			//camera_owner->create_component<Mover>();
			//camera_owner->create_component<MouseLooker>( 2.0f );
			camera = camera_owner->create_component<Camera>( 77.7f );
			//camera->look_at( Vec3::forward * 500.0f );
			camera->activate();
		}
		
		void update( float dt ) override
		{
			float time = _game->get_timer()->get_accumulated_seconds();
			auto inputs = _game->get_inputs();
			auto window = _game->get_window();

			//  get normalized mouse pos
			Vec2 window_size = window->get_size();
			Vec2 mouse_pos = inputs->get_mouse_pos();
			Vec2 normalized_mouse_pos = mouse_pos - window_size * 0.5f;
			normalized_mouse_pos /= window_size * 0.8f;
			normalized_mouse_pos *= 2.0f;
			normalized_mouse_pos.x = math::clamp( normalized_mouse_pos.x, -1.0f, 1.0f );
			normalized_mouse_pos.y = math::clamp( normalized_mouse_pos.y, -1.0f, 1.0f );
			//printf( "%f %f\n", normalized_mouse_pos.x, normalized_mouse_pos.y );

			Vec3 target_spaceship_location = Vec3::forward * previous_spaceship_location;
			target_spaceship_location += Vec3::up * normalized_mouse_pos.y * 3.5f;
			target_spaceship_location += Vec3::right * normalized_mouse_pos.x * 5.5f;
			
			previous_spaceship_location = Vec3::lerp(
				previous_spaceship_location,
				target_spaceship_location,
				dt * 5.0f
			);

			float move_speed =  dt * 2.0f;
			Vec3 movement = Vec3::forward * move_speed;
			previous_spaceship_location += movement;
			camera->transform->location += movement;
			camera->transform->is_matrix_dirty = true;

			//  location (idle feeling)
			Vec3 location = previous_spaceship_location;
			location += Vec3 { 
				math::cos( time * 0.4f ) * 0.1f,
				math::sin( time * 1.5f ) * 0.2f,
				math::cos( time * 1.8f ) * 0.3f
			};
			spaceship->transform->set_location( location );

			//  aim velocity (movement feeling)
			const float MAX_AIM = 500.0f;
			spaceship_aim_velocity.x = math::clamp( 
				spaceship_aim_velocity.x + inputs->mouse_delta.x,
				-MAX_AIM,
				MAX_AIM
			);
			spaceship_aim_velocity.y = math::clamp( 
				spaceship_aim_velocity.y + inputs->mouse_delta.y,
				-MAX_AIM,
				MAX_AIM
			);
			spaceship_aim_velocity = Vec2::approach( 
				spaceship_aim_velocity, 
				Vec2::zero, 
				dt * 200.0f
			);
			//printf( "%f %f\n", spaceship_aim_velocity.x, spaceship_aim_velocity.y );

			//  rotations
			Quaternion target_roll = Quaternion( 
				Vec3::unit_x, 
				-normalized_mouse_pos.x * math::PI * 0.05f
			  + -spaceship_aim_velocity.x / MAX_AIM * math::PI * 0.1f
			);
			Quaternion target_pitch = Quaternion(
				Vec3::unit_z,
				-normalized_mouse_pos.y * math::PI * 0.05f
			  + -spaceship_aim_velocity.y / MAX_AIM * math::PI * 0.1f
			);
			Quaternion rotation = spaceship->transform->rotation;
			rotation = Quaternion::lerp(
				rotation,
				SPACESHIP_ROTATION_OFFSET + target_roll + target_pitch,
				dt * 15.0f
			);
			spaceship->transform->set_rotation( rotation );

			if ( inputs->is_key_down( SDL_SCANCODE_SPACE ) )
			{
				if ( shoot_time <= 0.0f )
				{
					auto bullet = new Entity();
					bullet->transform->location = spaceship->transform->location + Vec3::forward * 1.5f;
					//bullet->transform->rotation = /*Quaternion::look_at( 
					//	bullet->transform->location,
					//	target_spaceship_location + spaceship->transform->get_forward() * 50.0f,
					//	Vec3::up
					//)*/Quaternion::look_at( Vec3::forward, Vec3::up );
					bullet->create_component<StylizedModelRenderer>( 
						Assets::get_model( "projectile" ), 
						Color::green
					);
					bullet->create_component<Projectile>();

					shoot_time = SHOOT_TIME;
					printf( "shoot\n" );
				}
			}
			shoot_time -= dt;
		}

	private:
		std::shared_ptr<Camera> camera;
		Entity* spaceship;
		Vec3 previous_spaceship_location;
		Quaternion SPACESHIP_ROTATION_OFFSET;// = Quaternion( Vec3::up, math::HALF_PI );

		float shoot_time = 0.0f;
		const float SHOOT_TIME = 0.1f;

		Vec2 spaceship_aim_velocity;
	};
}