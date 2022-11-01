#include "pacman.h"

#include "ghost.hpp"

PacMan::PacMan( Level* level )
	: Entity()
{
	//  create mover
	mover = new PlayerMover( this, level );
	mover->move_time = .016f;
	mover->rotate_towards_dir = true;

	//  create animator
	anim = new AnimSpriteRenderer( this, Assets::get_texture( "atlas.png" ), 16 );
	anim->gen_frames( Rect { 0.0f, 0.0f, 48.0f, 16.0f }, Vec2 { 16.0f, 16.0f } );
	anim->dest = { Level::TILE_SIZE / 2.0f, Level::TILE_SIZE / 2.0f, 16.0f, 16.0f };

	//  create collider
	new RectCollider( this, Rect { 2.0f, 2.0f, 4.0f, 4.0f } );
}

void PacMan::update_this( float dt )
{
	//  play animation if moving
	anim->is_playing = !mover->is_blocked;

	//  avoid full circle image when blocked
	if ( !anim->is_playing && anim->current_frame == 2 )
	{
		anim->set_current_frame( 1 );
	}
}

void PacMan::on_trigger_enter( Collider* other )
{
	Entity* ent = other->get_owner();

	if ( auto ghost = dynamic_cast<Ghost*>( ent ) )
	{
		switch ( ghost->mover->state )
		{
		case GhostState::SCATTER:
		case GhostState::CHASE:
			printf( "ded\n" );
			break;
		case GhostState::FLEE:
			ghost->mover->set_state( GhostState::EATEN );
			break;
		}
	}
}
