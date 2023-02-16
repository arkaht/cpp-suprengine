#pragma once

#include "../entities/ghost.fwd.h"
#include "../entities/pacman.h"
#include "mover.hpp"
#include <suprengine/ecs/components/renderers/anim-sprite-renderer.hpp>

namespace demo_pacman
{
	enum class GhostState
	{
		WAIT,
		SCATTER,
		CHASE,
		FLEE,
		EATEN,
	};

	class GhostMover : public Mover
	{
	private:
		Vec2 target { Vec2::zero };

		float current_flee_time { 0.0f };
		bool is_flee_ending { false };
		bool is_waiting_finished { false };
	public:
		const float FLEE_TIME { 8.0f };
		const float FLEE_END_TIME { 2.0f };  //  2 seconds before FLEE_TIME

		const float WAIT_MOVE_TIME { 0.02f };
		const float SLOW_MOVE_TIME { 0.03f };
		const float NORMAL_MOVE_TIME { 0.016f };
		const float FLEE_MOVE_TIME { 0.03f };
		const float EATEN_MOVE_TIME { 0.01f };

		int wait_id = 0;
		float wait_time = 1.0f;

		GhostState state { GhostState::SCATTER };
		Ghost* personality;
		PacMan* pacman { nullptr };
		AnimSpriteRenderer* anim { nullptr };

		GhostMover( Ghost* owner, Level* level, PacMan* pacman, AnimSpriteRenderer* anim, int priority_order = 0 );

		void update( float dt ) override;
		void update_target();

		void set_state( GhostState state );

		void reset_flee_timer();

		void on_moved() override;
		void on_next_pos_reached() override;
		Vec2 get_desired_dir() override;

		void debug_render( RenderBatch* render_batch ) override;
	};
}