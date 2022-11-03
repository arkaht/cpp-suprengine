#pragma once

#include <suprengine/ecs/components/renderers/sprite_renderer.hpp>
#include <suprengine/ecs/components/rect_collider.hpp>
#include <suprengine/ecs/entity.h>

#include "../layers.hpp"

using namespace suprengine;

class PowerPellet : public Entity
{
private:
	const float BLINK_TIME { 0.15f };
	float blink_time { 0.0f };
public:
	SpriteRenderer* sprite;

	PowerPellet()
	{
		layer = (uint32_t) Layers::POWERUP;
			
		sprite = new SpriteRenderer( this, Assets::get_texture( "atlas.png" ), 2 );
		sprite->source = { 192.0f, 16.0f, 8.0f, 8.0f };
		sprite->dest = { 4.0f, 4.0f, 8.0f, 8.0f };

		new RectCollider( this, { 0.0f, 0.0f, 8.0f, 8.0f } );
		collider->mask = (uint32_t) Layers::PACMAN;
	}

	void update_this( float dt ) override
	{
		if ( ( blink_time -= dt ) <= 0.0f )
		{
			blink_time = BLINK_TIME;

			sprite->is_rendered = !sprite->is_rendered;
		}
	}
};