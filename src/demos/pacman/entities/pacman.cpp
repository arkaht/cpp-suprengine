#include "pacman.h"

#include "ghost.hpp"
#include "../scenes/game-scene.hpp"

#include "../layers.hpp"
#include "../game-stats.hpp"

using namespace demo_pacman;

PacMan::PacMan( Level* level )
	: level( level ), Entity()
{
	layer = (uint32_t) Layers::PACMAN;

	//  create mover
	mover = new PlayerMover( this, level );
	mover->move_time = .016f;
	mover->rotate_towards_dir = true;

	//  create animator
	anim = new AnimSpriteRenderer( this, Assets::get_texture( "atlas.png" ), 6, 10 );
	anim->gen_frames( Rect { 0.0f, 0.0f, 224.0f, 16.0f }, Vec2 { 16.0f, 16.0f } );
	anim->dest = { Level::TILE_SIZE / 2.0f, Level::TILE_SIZE / 2.0f, 16.0f, 16.0f };
	anim->add_clip( "default", AnimClip { 0, 2, 1.0f / 16.0f } );
	anim->add_clip( "death", AnimClip { 3, 13, 1.0f / 8.0f } );
	anim->set_current_clip( "default" );

	//  create collider
	new RectCollider( this, Rect { 2.0f, 2.0f, 4.0f, 4.0f } );
	collider->mask = (uint32_t) Layers::GHOSTS
		| (uint32_t) Layers::POWERUP;

	//TIMER( 1.0f, win(); );
}

void PacMan::update_this( float dt )
{
	if ( is_dying ) return;

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

	GameStats& stats = GameStats::instance();
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
			stats.add_ghost_score( ghost );
			break;
		}
	}
	else if ( auto pellet = dynamic_cast<PowerPellet*>( ent ) )
	{
		pellet->kill();
		GameManager::flee_all();

		stats.add_score( 50 );
		if ( --stats.remaining_dots <= 0 )
			win();
	}
	else if ( auto dot = dynamic_cast<PacDot*>( ent ) )
	{
		dot->kill();

		stats.add_score( 10 );
		if ( --stats.remaining_dots <= 0 )
			win();
	}
}

void PacMan::die()
{
	if ( is_dying ) return;

	printf( "u'r dead, not a big surprise\n" );

	//  setup variables
	is_dying = true;

	//  wait some time for anim
	TIMER( DEATH_WAIT_TIME, {
		//  reset rotation
		transform->rotation = 0.0f;

		//  death animation
		anim->set_current_clip( "death" );
		anim->is_looping = false;
		anim->is_playing = true;

		//  kill ghosts
		GameManager::kill_all();

		auto& clip = anim->clips[anim->current_clip];
		TIMER( clip.time_per_frame * ( clip.end_frame - clip.start_frame + 1 ), {
			//  disable renderer
			anim->is_rendered = false;
			
			//  restart
			TIMER( DEATH_RESTART_TIME, {
				GameStats& stats = GameStats::instance();
				stats.score = 0;

				game->set_scene( new GameScene() );
			} );
		} );
	} );

	//  setup anim
	anim->is_playing = false;


	//  disable mover
	mover->is_updated = false;

	//  disable ghosts mover
	for ( auto ghost : GameManager::ghosts )
	{
		ghost->mover->is_updated = false;
	}
}

void PacMan::win()
{
	//  disable self
	state = EntityState::PAUSED;

	//  disable ghosts
	for ( auto ghost : GameManager::ghosts )
	{
		ghost->set_state( EntityState::PAUSED );
	}

	TIMER( 2.0f, {
		//  kill ghosts
		GameManager::kill_all();

		//  blink level
		level->blink();

		//  reload scene
		TIMER( 2.0f, { 
			game->set_scene( new GameScene() ); 
		} );
	} );
}