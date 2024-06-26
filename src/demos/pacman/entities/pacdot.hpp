#pragma once

#include <suprengine/ecs/components/renderers/sprite-renderer.hpp>
#include <suprengine/ecs/components/rect-collider.hpp>
#include <suprengine/ecs/entity.h>

#include "../layers.hpp"

using namespace suprengine;

namespace demo_pacman
{
	class PacDot : public Entity
	{
	public:
		PacDot()
		{
			layer = (uint32_t) Layers::POWERUP;

			auto sprite = new SpriteRenderer( this, Assets::get_texture( "atlas.png" ), 2 );
			sprite->source = { 192.0f, 24.0f, 8.0f, 8.0f };
			sprite->dest = { 4.0f, 4.0f, 8.0f, 8.0f };

			new RectCollider( this, { 0.0f, 0.0f, 8.0f, 8.0f } );
			collider->mask = (uint32_t) Layers::PACMAN;
		}
	};
}