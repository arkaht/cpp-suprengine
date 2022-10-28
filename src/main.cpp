#include "suprengine/game.h"

#include <suprengine/ecs/entity.h>
#include <suprengine/ecs/components/renderers/anim_sprite_renderer.hpp>

using namespace suprengine;

int main( int arg_count, char** args )
{
	//  create game
	auto& game = Game::instance();
	if ( !game.initialize( "my-engine-demo", 1280, 720 ) )
	{
		return 0;
	}

	//  load assets
	Assets::set_render_batch( game.get_render_batch() );

	//  prepare some entities
	Entity* ent_sprite = new Entity();
	ent_sprite->transform->pos = { 160.0f, 160.0f };
	//new RectRenderer( my_ent, Rect { -64.0f, -64.0f, 128.0f, 128.0f }, Color::blue );
	auto sprite = new SpriteRenderer( ent_sprite, Assets::get_texture( "atlas.png" ) );
	sprite->source = { 0.0f, 0.0f, 16.0f, 16.0f };
	sprite->dest = { 0.0f, 0.0f, 16.0f, 16.0f };

	Entity* ent_anim = new Entity();
	ent_anim->transform->pos = { 160.0f + 24.0f, 160.0f };
	auto anim_sprite = new AnimSpriteRenderer( ent_anim, Assets::get_texture( "atlas.png" ) );
	anim_sprite->add_frame( Rect { 0.0f, 0.0f, 16.0f, 16.0f } );
	anim_sprite->add_frame( Rect { 16.0f, 0.0f, 16.0f, 16.0f } );
	anim_sprite->add_frame( Rect { 32.0f, 0.0f, 16.0f, 16.0f } );
	anim_sprite->dest = { 0.0f, 0.0f, 16.0f, 16.0f };

	Entity* pacman_dead_anim = new Entity();
	pacman_dead_anim->transform->pos = { 160.0f + 48.0f, 160.0f };
	anim_sprite = new AnimSpriteRenderer( pacman_dead_anim, Assets::get_texture( "atlas.png" ), 8 );
	anim_sprite->gen_frames( Rect { 32.0f, 0.0f, 192.0f, 16.0f }, Vec2 { 16.0f, 16.0f } );
	anim_sprite->dest = { 0.0f, 0.0f, 16.0f, 16.0f };
	anim_sprite->is_looping = false;

	Entity* ghost_frightened_anim = new Entity();
	ghost_frightened_anim->transform->pos = { 160.0f + 48.0f, 160.0f + 24.0f };
	anim_sprite = new AnimSpriteRenderer( ghost_frightened_anim, Assets::get_texture( "atlas.png" ) );
	anim_sprite->gen_frames( Rect { 128.0f, 16.0f, 64.0f, 16.0f }, Vec2 { 16.0f, 16.0f } );
	anim_sprite->dest = { 0.0f, 0.0f, 16.0f, 16.0f };
	anim_sprite->set_fps( 16 );

	//  game loop
	game.loop();

	return 0;
}