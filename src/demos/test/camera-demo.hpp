#pragma once
#include <suprengine/component.h>

#include <suprengine/entity.h>
#include <suprengine/components/spring-arm.hpp>
#include <suprengine/components/mover.hpp>
#include <suprengine/components/mouse-follower.hpp>
#include <suprengine/components/mouse-looker.hpp>
#include <suprengine/components/target-rotator.h>
#include <suprengine/input-manager.h>

using namespace suprengine;

namespace test
{
	class CameraDemo : public Entity
	{
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
			mover = create_component<Mover>();
			mouse_looker = create_component<MouseLooker>( 1.0f );
			spring_arm = create_component<SpringArm>( player->transform );
			target_rotator = create_component<TargetRotator>( player->transform );

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
			auto inputs = _game->get_inputs();

			if ( inputs->is_key_just_pressed( SDL_SCANCODE_1 ) )
				set_mode( CameraMode::FPS );
			else if ( inputs->is_key_just_pressed( SDL_SCANCODE_2 ) )
				set_mode( CameraMode::TPS );
			else if ( inputs->is_key_just_pressed( SDL_SCANCODE_3 ) )
				set_mode( CameraMode::STATIC );
		}

	private:
		Entity* player { nullptr };

		std::shared_ptr<SpringArm> spring_arm;
		std::shared_ptr<Mover> mover;
		std::shared_ptr<MouseLooker> mouse_looker;
		std::shared_ptr<TargetRotator> target_rotator;

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
					player->state = EntityState::ACTIVE;
					spring_arm->should_update = true;
					break;
				case CameraMode::STATIC:
					player->state = EntityState::ACTIVE;
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
					player->state = EntityState::PAUSED;
					spring_arm->should_update = false;
					break;
				case CameraMode::STATIC:
					player->state = EntityState::PAUSED;
					target_rotator->should_update = false;
					break;
			}
		}
	};
}