#pragma once

#include "../components/ghost_mover.h"
#include <suprengine/ecs/components/renderers/anim_sprite_renderer.hpp>

#include "ghost_manager.h"

#include "../layers.hpp"

class Ghost : public Entity
{
public:
	Color color;
	Level* level { nullptr };
	PacMan* pacman { nullptr };

	GhostMover* mover { nullptr };
	AnimSpriteRenderer* anim { nullptr };

	Ghost( Level* level, PacMan* pacman ) : level( level ), pacman( pacman ), Entity()
	{
		layer = (uint32_t) Layers::GHOSTS;

		//  create animator
		anim = new AnimSpriteRenderer( this, Assets::get_texture( "atlas.png" ), 6, Color::white, 1 );
		anim->dest = { Level::TILE_SIZE / 2, Level::TILE_SIZE / 2, 16.0f, 16.0f };
		
		//  gen flee frames & clips
		anim->gen_frames( Rect { 128.0f, 16.0f, 64.0f, 16.0f }, Vec2 { 16.0f, 16.0f } );
		anim->add_clip( "flee", AnimClip { 0, 1 } );
		anim->add_clip( "flee_end", AnimClip { 0, 3 } );

		//  gen eaten frames & clips
		anim->gen_frames( Rect { 128.0f, 32.0f, 64.0f, 16.0f }, Vec2 { 16.0f, 16.0f } );
		anim->add_clip( "eaten_right", AnimClip { 4, 4 } );
		anim->add_clip( "eaten_left", AnimClip { 5, 5 } );
		anim->add_clip( "eaten_up", AnimClip { 6, 6 } );
		anim->add_clip( "eaten_down", AnimClip { 7, 7 } );
		
		//  add default clips
		anim->add_clip( "right", AnimClip { 8, 9 } );
		anim->add_clip( "left", AnimClip { 10, 11 } );
		anim->add_clip( "up", AnimClip { 12, 13 } );
		anim->add_clip( "down", AnimClip { 14, 15 } );

		//  create mover
		mover = new GhostMover( this, level, pacman, anim );

		//  create collider
		new RectCollider( this, Rect { 0.0f, 0.0f, 8.0f, 8.0f } );
		collider->mask = (uint32_t) Layers::PACMAN;

		GhostManager::add_ghost( this );
	}

	virtual Vec2 get_scatter_target() = 0;
	virtual Vec2 get_chase_target() = 0;
};
