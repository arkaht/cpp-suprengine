#pragma once

#include "../entities/ghost.fwd.h"
#include "../entities/pacman.h"
#include "mover.hpp"
#include <suprengine/ecs/components/renderers/anim_sprite_renderer.hpp>

enum class GhostState
{
	HOME,
	SCATTER,
	CHASE,
	FLEE,
	EATEN,
};

class GhostMover : public Mover
{
private:
	Vec2 target { Vec2::zero };

	const float FLEE_TIME { 8.0f };
	const float FLEE_END_TIME { 2.0f };  //  2 seconds before FLEE_TIME
	
	const float NORMAL_MOVE_TIME { 0.016f };
	const float FLEE_MOVE_TIME { 0.018f };
	const float EATEN_MOVE_TIME { 0.01f };

	float current_flee_time { 0.0f };
	bool is_flee_ending { false };
public:
	GhostState state { GhostState::SCATTER };
	Ghost* personality;
	PacMan* pacman { nullptr };
	AnimSpriteRenderer* anim { nullptr };

	GhostMover( Ghost* owner, Level* level, PacMan* pacman, AnimSpriteRenderer* anim, int priority_order = 0 );

	void update( float dt ) override;
	void update_target();

	void set_state( GhostState state );

	void on_next_pos_reached() override;
	Vec2 get_desired_dir() override;

	void debug_render( RenderBatch* render_batch ) override;
};