#pragma once
#include <suprengine/ecs/components/renderers/sprite_renderer.hpp>

#include "power_pellet.hpp"
#include "pacdot.hpp"

#include <vector>

using namespace suprengine;

const int TUNNELS_COUNT { 2 };

class Level : public Entity
{
private:
	int width { 0 }, height { 0 };
	std::vector<bool> tiles;

	const float BLINK_TIME { 0.25f };
	float blink_time { 0.0f };

	const std::string TEXTURE_PATH { "level.png" };

	const uint32_t WALL_COLOR_PIXEL { Color { 33, 33, 255 }.to_pixel() };
	const uint32_t DOT_COLOR_PIXEL { Color { 255, 183, 174 }.to_pixel() };

	Vec2 tunnels_pos[TUNNELS_COUNT]
	{
		{ 0.0f, 14.0f }, { 27.0f, 14.0f },
	};
	Vec2 waits_pos[6]
	{
		{ 11.5f, 13.5f }, { 11.5f, 14.5f },
		{ 13.5f, 14.5f }, { 13.5f, 13.5f },
		{ 15.5f, 13.5f }, { 15.5f, 14.5f },
	};
	Vec2 ghost_door_pos { 13.5f, 11.0f },
		ghost_house_pos { 13.5f, 13.0f };

	void gen_tiles();
	void gen_blink_texture( SDL_Surface* surface );
public:
	static const int TILE_SIZE { 8 };

	bool is_blinking { false };

	Texture* blink_texture { nullptr };
	Texture* normal_texture { nullptr };
	SpriteRenderer* sprite { nullptr };
	SpriteRenderer* sprite_door { nullptr };

	Level() : Entity()
	{
		sprite = new SpriteRenderer( this, nullptr );
		sprite->origin = Vec2::zero;

		sprite_door = new SpriteRenderer( this, Assets::get_texture( "door.png" ), 3 );
		sprite_door->dest.x = 104.0f, sprite_door->dest.y = 100.0f;
		sprite_door->origin = Vec2::zero;

		gen_tiles();
	}

	~Level()
	{
		//  free blink texture
		if ( blink_texture != nullptr )
		{
			delete blink_texture;
		}

		//  free normal texture
		if ( normal_texture == nullptr )
		{
			delete normal_texture;
		}
	}

	void blink()
	{
		is_blinking = true;
		sprite_door->is_rendered = false;
	}

	void update_this( float dt ) override
	{
		if ( !is_blinking ) return;

		if ( ( blink_time -= dt ) <= 0.0f )
		{
			sprite->texture = sprite->texture == normal_texture ? blink_texture : normal_texture;
			blink_time = BLINK_TIME;
		}
	}

	int get_tile_id( int x, int y )
	{
		return y * width + x;
	}

	bool is_wall_tile( int x, int y )
	{
		return tiles[get_tile_id( x, y )];
	}

	int get_width() const { return width; }
	int get_height() const { return height; }

	int get_tunnel_id( Vec2 pos )
	{
		for ( int i = 0; i < TUNNELS_COUNT; i++ )
		{
			if ( tunnels_pos[i] == pos )
			{
				return i;
			}
		}

		return -1;
	}

	Vec2 get_tunnel( int id ) const { return tunnels_pos[id]; }
	Vec2 get_wait_pos( int id ) const { return waits_pos[id]; }
	Vec2 get_ghost_door_pos() const { return ghost_door_pos; }
	Vec2 get_ghost_house_pos() const { return ghost_house_pos; }


	void debug_render( RenderBatch* render_batch ) override
	{
		float tile_size = Level::TILE_SIZE;

		for ( int ty = 0; ty < height; ty++ )
		{
			for ( int tx = 0; tx < width; tx++ )
			{
				Rect rect {
					tx * tile_size,
					ty * tile_size,
					tile_size,
					tile_size,
				};

				//  draw grid
				render_batch->draw_rect( DrawType::LINE, rect, Color { 255, 255, 255, 27 } );

				//  draw wall
				if ( is_wall_tile( tx, ty ) )
				{
					render_batch->draw_rect( DrawType::FILL, rect, Color { 127, 127, 0, 88 } );
				}
			}
		}
	}
};