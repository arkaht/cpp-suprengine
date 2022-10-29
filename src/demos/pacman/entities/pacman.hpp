#pragma once

#include "../components/mover.hpp"
#include <suprengine/ecs/components/renderers/anim_sprite_renderer.hpp>

class PacMan : public Entity
{
public:
	Mover* mover { nullptr };

	PacMan( Level* level ) : Entity()
	{
		mover = new Mover( this, level );
		mover->move_time = .2f;

		auto anim = new AnimSpriteRenderer( this, Assets::get_texture( "atlas.png" ), 16 );
		anim->gen_frames( Rect { 0.0f, 0.0f, 48.0f, 16.0f }, Vec2 { 16.0f, 16.0f } );
		anim->add_frame( Rect { 16.0f, 0.0f, 16.0f, 16.0f } );
		anim->dest = { 0.0f, 0.0f, 16.0f, 16.0f };
	}

	void update_this( float dt ) override
	{
		if ( game->is_key_pressed( SDL_SCANCODE_W ) || game->is_key_pressed( SDL_SCANCODE_UP ) )
		{
			mover->try_set_dir( Int2_16b::up );
		}
		else if ( game->is_key_pressed( SDL_SCANCODE_S ) || game->is_key_pressed( SDL_SCANCODE_DOWN ) )
		{
			mover->try_set_dir( Int2_16b::down );
		}
		else if ( game->is_key_pressed( SDL_SCANCODE_A ) || game->is_key_pressed( SDL_SCANCODE_LEFT ) )
		{
			mover->try_set_dir( Int2_16b::left );
		}
		else if ( game->is_key_pressed( SDL_SCANCODE_D ) || game->is_key_pressed( SDL_SCANCODE_RIGHT ) )
		{
			mover->try_set_dir( Int2_16b::right );
		}
	}
};