#include "pacman.h"

#include "ghost.hpp"
#include "../scenes/game_scene.hpp"

PacMan::PacMan( Level* level )
	: Entity()
{
	//  create mover
	mover = new PlayerMover( this, level );
	mover->move_time = .016f;
	mover->rotate_towards_dir = true;

	//  create animator
	anim = new AnimSpriteRenderer( this, Assets::get_texture( "atlas.png" ) );
	anim->gen_frames( Rect { 0.0f, 0.0f, 224.0f, 16.0f }, Vec2 { 16.0f, 16.0f } );
	anim->dest = { Level::TILE_SIZE / 2.0f, Level::TILE_SIZE / 2.0f, 16.0f, 16.0f };
	anim->add_clip( "default", AnimClip { 0, 2, 1.0f / 16.0f } );
	anim->add_clip( "death", AnimClip { 3, 13, 1.0f / 8.0f } );
	anim->set_current_clip( "default" );

	//  create collider
	new RectCollider( this, Rect { 0.0f, 0.0f, 8.0f, 8.0f } );
}

void PacMan::update_this( float dt )
{
	if ( is_dying )
	{
		//  wait some time before playing animation
		if ( is_waiting_dying )
		{
			if ( ( death_wait_time -= dt ) <= 0.0f )
			{
				is_waiting_dying = false;

				anim->set_current_clip( "death" );
				anim->is_looping = false;
				anim->is_playing = true;
				GhostManager::delete_all();
			}
		}
		//  remove render on finished
		else if ( !anim->is_playing )
		{
			delete anim;

			TIMER( 
				DEATH_RESTART_TIME,
				{
					game->set_scene( new GameScene() );
				} 
			);
		}
	}
	else
	{
		//  play animation if moving
		anim->is_playing = !mover->is_blocked;

		//  avoid full circle image when blocked
		if ( !anim->is_playing && anim->current_frame == 2 )
		{
			anim->set_current_frame( 1 );
		}
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
			die();
			break;
		case GhostState::FLEE:
			ghost->mover->set_state( GhostState::EATEN );
			break;
		}
	}
}

void PacMan::die()
{
	if ( is_dying ) return;

	printf( "u'r dead, not a big surprise\n" );

	//  setup variables
	is_dying = true, is_waiting_dying = true;

	//  wait some time for anim
	death_wait_time = DEATH_WAIT_TIME;

	//  setup anim
	anim->is_playing = false;

	//  reset rotation
	transform->rotation = 0.0f;

	//  disable mover
	mover->is_updated = false;

	//  disable ghosts mover
	for ( auto ghost : GhostManager::ghosts )
	{
		ghost->mover->is_updated = false;
	}
}
