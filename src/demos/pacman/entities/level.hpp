#pragma once
#include <suprengine/ecs/components/renderers/sprite_renderer.hpp>

#include <vector>

using namespace suprengine;

const int TUNNELS_COUNT { 2 };

class Level : public Entity
{
private:
	int width { 0 }, height { 0 };
	std::vector<bool> tiles;

	const std::string TEXTURE_PATH { "level.png" };

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
public:
	static const int TILE_SIZE { 8 };

	Level() : Entity()
	{
		auto sprite = new SpriteRenderer( this, Assets::get_texture( TEXTURE_PATH ) );
		sprite->origin = Vec2::zero;

		gen_tiles();
	}

	void gen_tiles()
	{
		//  load image as surface
		SDL_Surface* surface = Texture::load_surface( Assets::get_path() + TEXTURE_PATH );

		//  reserve tiles
		width = surface->w / TILE_SIZE, height = surface->h / TILE_SIZE;
		tiles.resize( (size_t)( surface->w * surface->h ), false );

		//  read image pixels
		for ( int ty = 0; ty < height; ty++ )
		{
			for ( int tx = 0; tx < width; tx++ )
			{
				for ( int y = 0; y < TILE_SIZE; y++ )
				{
					bool is_breaking = false;

					for ( int x = 0; x < TILE_SIZE; x++ )
					{
						//  check pixel color
						uint32_t pixel = Texture::get_pixel_at( surface, tx * TILE_SIZE + x, ty * TILE_SIZE + y );
						if ( pixel != 0 )
						{
							//  mark as a wall
							tiles[get_tile_id( tx, ty )] = true;

							//  skip to next tile
							is_breaking = true;
							break;
						}
					}

					if ( is_breaking ) 
						break; 
				}
			}
		}

		//  release surface
		SDL_FreeSurface( surface );
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