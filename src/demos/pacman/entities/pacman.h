#pragma once

#include "../components/player_mover.hpp"
#include <suprengine/ecs/components/renderers/anim_sprite_renderer.hpp>
#include <suprengine/ecs/components/rect_collider.hpp>

class PacMan : public Entity
{
private:
	const float DEATH_WAIT_TIME { 1.0f };
	const float DEATH_RESTART_TIME { 1.0f };

	bool is_dying { false };
public:
	Level* level;

	PlayerMover* mover { nullptr };
	AnimSpriteRenderer* anim { nullptr };

	PacMan( Level* level );

	void update_this( float dt ) override;

	void on_trigger_enter( Collider* other ) override;

	void die();
	void win();
};