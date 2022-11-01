#pragma once

#include "../components/player_mover.hpp"
#include "../components/debug_mover_renderer.hpp"
#include <suprengine/ecs/components/renderers/anim_sprite_renderer.hpp>

class PacMan : public Entity
{
public:
	PlayerMover* mover { nullptr };
	AnimSpriteRenderer* anim { nullptr };

	PacMan( Level* level ) : Entity()
	{
		mover = new PlayerMover( this, level );
		mover->move_time = .016f;
		mover->rotate_towards_dir = true;

		//new DebugMoverRenderer( this, mover );

		anim = new AnimSpriteRenderer( this, Assets::get_texture( "atlas.png" ), 16 );
		anim->gen_frames( Rect { 0.0f, 0.0f, 48.0f, 16.0f }, Vec2 { 16.0f, 16.0f } );
		//anim->add_frame( Rect { 16.0f, 0.0f, 16.0f, 16.0f } );
		anim->dest = { Level::TILE_SIZE / 2, Level::TILE_SIZE / 2, 16.0f, 16.0f };
	}

	void update_this( float dt ) override
	{
		//  play animation if moving
		anim->is_playing = mover->direction.value != 0;
	}
};