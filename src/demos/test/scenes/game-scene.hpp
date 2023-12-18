#pragma once

#include <suprengine/scene.h>
#include <suprengine/components/colliders/box-collider.hpp>
#include <suprengine/components/mover.hpp>
#include <suprengine/components/mouse-looker.hpp>
#include <suprengine/components/renderers/model-renderer.hpp>

#include "camera-demo.hpp"

using namespace suprengine;

namespace test
{
	class GameScene : public Scene
	{
	public:
		void init() override 
		{
			_engine->get_inputs()->set_relative_mouse_mode( true );

			RenderBatch* render_batch = _engine->get_render_batch();
			render_batch->set_background_color( Color::from_0x( 0x252627FF ) );

			Model* cube_model = Assets::get_model( "suprengine::cube" );
			Model* cylinder_model = Assets::get_model( "suprengine::cylinder" );
			Model* sphere_model = Assets::get_model( "suprengine::sphere" );

			//  setup ground
			auto ground = new Entity();
			ground->transform->scale = Vec3 { 100.0f, 100.0f, 1.0f };
			ground->create_component<ModelRenderer>( cube_model, "simple-mesh" );
			ground->create_component<BoxCollider>( Box::HALF );

			//  setup sphere
			auto sphere = new Entity();
			sphere->transform->location = Vec3 { 0.0f, 0.0f, 4.0f };
			sphere->create_component<ModelRenderer>( sphere_model, "simple-mesh", Color::blue );

			//  setup cube
			auto cube = new Entity();
			cube->transform->location = Vec3 { 0.0f, 4.0f, 4.0f };
			cube->create_component<ModelRenderer>( cube_model, "simple-mesh", Color::green );

			//  setup cylinder
			auto cylinder = new Entity();
			cylinder->transform->location = Vec3 { 4.0f, 0.0f, 4.0f };
			cylinder->create_component<ModelRenderer>( cylinder_model, "simple-mesh", Color::red );
			
			//  setup camera
			auto camera_owner = new CameraDemo( sphere );
			camera_owner->transform->location = Vec3 { 5.0f, 3.0f, 7.0f };
			camera_owner->transform->look_at( cylinder->transform->location );
			camera_owner->create_component<Mover>();
			//camera_owner->create_component<MouseLooker>( 2.0f );
			auto camera = camera_owner->create_component<Camera>( CameraProjectionSettings {} );
			camera->activate();
		}

		void update( float dt ) override
		{
			float time = _engine->get_timer()->get_accumulated_seconds();

			RenderBatch* render_batch = _engine->get_render_batch();
			render_batch->set_ambient_direction(
				Vec3 {
					cosf( time ),
					cosf( time ) * sinf( time ),
					sinf( time ),
				}
			);
		}
	};
}