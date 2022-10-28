#pragma once

#include <suprengine/game.h>

#include <suprengine/ecs/entity.h>
#include <suprengine/ecs/components/renderers/anim_sprite_renderer.hpp>

using namespace suprengine;

void run()
{
	auto& game = Game::instance();
	if ( !game.initialize( "pac-man" ) ) return;

	auto pacman = new Entity();
	pacman->transform->pos = { 160.0f + 48.0f, 160.0f };
	auto anim_sprite = new AnimSpriteRenderer( pacman, Assets::get_texture( "atlas.png" ), 20 );
	anim_sprite->gen_frames( Rect { 0.0f, 0.0f, 48.0f, 16.0f }, Vec2 { 16.0f, 16.0f } );
	anim_sprite->add_frame( Rect { 16.0f, 0.0f, 16.0f, 16.0f } );
	anim_sprite->dest = { 0.0f, 0.0f, 16.0f, 16.0f };

	Entity* ghost_frightened_anim = new Entity();
	ghost_frightened_anim->transform->pos = { 160.0f + 48.0f, 160.0f + 24.0f };
	anim_sprite = new AnimSpriteRenderer( ghost_frightened_anim, Assets::get_texture( "atlas.png" ), 6, Color::green );
	anim_sprite->gen_frames( Rect { 128.0f, 16.0f, 64.0f, 16.0f }, Vec2 { 16.0f, 16.0f } );
	anim_sprite->dest = { 0.0f, 0.0f, 16.0f, 16.0f };
	anim_sprite->set_fps( 16 );

	game.loop();
}