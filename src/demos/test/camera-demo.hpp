#pragma once

#include <suprengine/core/component.h>
#include <suprengine/core/entity.h>
#include <suprengine/input/input-manager.h>

#include <suprengine/components/looker.hpp>
#include <suprengine/components/mouse-follower.hpp>
#include <suprengine/components/mover.hpp>
#include <suprengine/components/spring-arm.hpp>
#include <suprengine/components/target-rotator.h>

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

		CameraDemo(
			SafePtr<Entity> player,
			InputAction<Vec2>* move_action,
			InputAction<Vec2>* look_action,
			InputAction<bool>* sprint_action,
			InputAction<float>* vertical_action
		)
			: player( player ),
			  move_action( move_action ),
			  look_action( look_action ),
			  sprint_action( sprint_action ),
			  vertical_action( vertical_action )
		{}

		void setup() override
		{
			mover = create_component<Mover>( move_action, sprint_action, vertical_action );
			mouse_looker = create_component<Looker>( look_action, 3.0f );
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
			auto inputs = Engine::instance().get_inputs();

			if ( inputs->is_key_just_pressed( SDL_SCANCODE_1 ) )
				set_mode( CameraMode::FPS );
			else if ( inputs->is_key_just_pressed( SDL_SCANCODE_2 ) )
				set_mode( CameraMode::TPS );
			else if ( inputs->is_key_just_pressed( SDL_SCANCODE_3 ) )
				set_mode( CameraMode::STATIC );
		}

	private:
		SafePtr<Entity> player;

		SharedPtr<SpringArm> spring_arm;
		SharedPtr<Mover> mover;
		SharedPtr<Looker> mouse_looker;
		SharedPtr<TargetRotator> target_rotator;

		CameraMode current_mode = CameraMode::FPS;

		InputAction<Vec2>* move_action = nullptr;
		InputAction<Vec2>* look_action = nullptr;
		InputAction<bool>* sprint_action = nullptr;
		InputAction<float>* vertical_action = nullptr;

		void _enable_mode( CameraMode mode )
		{
			switch ( mode )
			{
				case CameraMode::FPS:
					mover->is_active = true;
					mouse_looker->is_active = true;
					break;
				case CameraMode::TPS:
					player->state = EntityState::Active;
					spring_arm->is_active = true;
					break;
				case CameraMode::STATIC:
					player->state = EntityState::Active;
					target_rotator->is_active = true;
					break;
			}
		}

		void _disable_mode( CameraMode mode )
		{
			switch ( mode )
			{
				case CameraMode::FPS:
					mover->is_active = false;
					mouse_looker->is_active = false;
					break;
				case CameraMode::TPS:
					player->state = EntityState::Paused;
					spring_arm->is_active = false;
					break;
				case CameraMode::STATIC:
					player->state = EntityState::Paused;
					target_rotator->is_active = false;
					break;
			}
		}
	};
}