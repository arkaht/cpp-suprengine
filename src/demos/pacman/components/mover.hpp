#pragma once
#include <suprengine/ecs/component.h>

#include <suprengine/ecs/entity.h>
#include <suprengine/ecs/components/transform2.hpp>
#include <suprengine/vec2.h>

#include "../entities/level.hpp"
#include "../int2_16b.h"

using namespace suprengine;

class Mover : public Component
{
public:
	Int2_16b direction { Int2_16b::right };
	float move_time { 0.5f };
	float current_move_time { 0.0f };
	float rotation { 0.0f };

	Level* level { nullptr };

	Mover( Entity* owner, Level* level, int priority_order = 0 )
		: level( level ), Component( owner, priority_order ) {}

	void update( float dt ) override
	{
		if ( ( current_move_time += dt ) >= move_time )
		{
			Vec2 new_pos = get_pos().to_vec2() + direction.to_vec2();
			if ( level->is_wall_tile( new_pos.x, new_pos.y ) ) return;

			Transform2* transf = owner->transform;
			transf->pos = new_pos * Level::TILE_SIZE;
			transf->rotation = direction.get_angle();

			current_move_time = 0.0f;
		}
	}

	bool try_set_dir( Int2_16b dir )
	{
		Int2_16b pos = get_pos();
		if ( level->is_wall_tile( pos.get_x() + dir.get_x(), pos.get_y() + dir.get_y() ) ) return false;

		direction = dir;
		return true;
	}

	void set_pos( Int2_16b pos )
	{
		owner->transform->pos.x = pos.get_x();
		owner->transform->pos.y = pos.get_y();
	}

	Int2_16b get_pos() const
	{
		return Int2_16b(
			owner->transform->pos.x / Level::TILE_SIZE,
			owner->transform->pos.y / Level::TILE_SIZE
		);
	}
};