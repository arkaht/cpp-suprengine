#pragma once

#include "../entities/ghost.fwd.h"
#include "../entities/pacman.hpp"
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

//enum class GhostPersonality
//{
//	BLINKY,  //  targets PAC-MAN
//	PINKY,  //  targets PAC-MAN forward direction by 4 tiles
//	INKY,  //  targets mirrored position of BLINKY from PAC-MAN
//	CLYDE,  //  targets PAC-MAN but flee him if within 8 tiles from him
//};

class GhostMover : public Mover
{
private:
	Vec2 target { Vec2::zero };
public:
	GhostState state { GhostState::SCATTER };
	Ghost* personality;
	PacMan* pacman { nullptr };
	AnimSpriteRenderer* anim { nullptr };

	GhostMover( Ghost* owner, Level* level, PacMan* pacman, AnimSpriteRenderer* anim, int priority_order = 0 );

	void update( float dt ) override;
	void update_target();

	Vec2 get_desired_dir() override;

	void debug_render( RenderBatch* render_batch ) override;
};