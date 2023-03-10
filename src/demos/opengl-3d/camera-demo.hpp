#pragma once
#include <suprengine/ecs/component.h>

#include <suprengine/ecs/entity.h>
#include <suprengine/ecs/components/spring-arm.hpp>
#include <suprengine/ecs/components/mover.hpp>
#include <suprengine/ecs/components/mouse-follower.hpp>
#include <suprengine/ecs/components/mouse-looker.hpp>
#include <suprengine/ecs/components/target-rotator.h>
#include <suprengine/input-manager.h>

using namespace suprengine;

namespace demo_opengl3d
{
	class CameraDemo : public Entity
	{
	private:
		Entity* player { nullptr };

		SpringArm* spring_arm { nullptr };
		Mover* mover { nullptr };
		MouseLooker* mouse_looker { nullptr };
		TargetRotator* target_rotator { nullptr };
	public:
		enum class CameraMode
		{
			FPS,
			TPS,
			STATIC,
		};

		CameraDemo( Entity* player ) 
			: player( player ), Entity()
		{
			spring_arm = new SpringArm( this, player->transform );
			mover = new Mover( this );
			mouse_looker = new MouseLooker( this, 1.0f );
			target_rotator = new TargetRotator( this, player->transform );

			//  disable all modes
			_disable_mode( CameraMode::FPS );
			_disable_mode( CameraMode::TPS );
			_disable_mode( CameraMode::STATIC );

			//  enable current mode
			_enable_mode( current_mode );
		}

		void set_mode( CameraMode mode )
		{
			_disable_mode( current_mode );
			current_mode = mode;
			_enable_mode( current_mode );
		}

		void update_this( float dt ) override
		{
			InputManager* inputs = game->get_inputs();
			if ( inputs->is_key_pressed( SDL_SCANCODE_1 ) )
				set_mode( CameraMode::FPS );
			else if ( inputs->is_key_pressed( SDL_SCANCODE_2 ) )
				set_mode( CameraMode::TPS );
			else if ( inputs->is_key_pressed( SDL_SCANCODE_3 ) )
				set_mode( CameraMode::STATIC );
		}

	private:
		CameraMode current_mode = CameraMode::TPS;

		void _enable_mode( CameraMode mode )
		{
			switch ( mode )
			{
				case CameraMode::FPS:
					mover->should_update = true;
					mouse_looker->should_update = true;
					break;
				case CameraMode::TPS:
					player->set_state( EntityState::ACTIVE );
					spring_arm->should_update = true;
					break;
				case CameraMode::STATIC:
					player->set_state( EntityState::ACTIVE );
					target_rotator->should_update = true;
					break;
			}
		}

		void _disable_mode( CameraMode mode )
		{
			switch ( mode )
			{
				case CameraMode::FPS:
					mover->should_update = false;
					mouse_looker->should_update = false;
					break;
				case CameraMode::TPS:
					player->set_state( EntityState::PAUSED );
					spring_arm->should_update = false;
					break;
				case CameraMode::STATIC:
					player->set_state( EntityState::PAUSED );
					target_rotator->should_update = false;
					break;
			}
		}
	};
}