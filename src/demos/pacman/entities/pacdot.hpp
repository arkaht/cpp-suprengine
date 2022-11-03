#pragma once

#include <suprengine/ecs/components/renderers/sprite_renderer.hpp>
#include <suprengine/ecs/components/rect_collider.hpp>
#include <suprengine/ecs/entity.h>

#include "ghost_manager.h"

using namespace suprengine;

class PacDot : public Entity
{
public:
	PacDot()
	{
		auto sprite = new SpriteRenderer( this, Assets::get_texture( "atlas.png" ) );
		sprite->source = { 192.0f, 24.0f, 8.0f, 8.0f };
		sprite->dest = { 4.0f, 4.0f, 8.0f, 8.0f };

		new RectCollider( this, { 0.0f, 0.0f, 8.0f, 8.0f } );
	}
};