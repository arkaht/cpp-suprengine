#include "level.h"

#include "../game_stats.hpp"

void Level::gen_tiles()
{
	//  load image as surface
	SDL_Surface* surface = Texture::load_surface( Assets::get_path() + TEXTURE_PATH );

	//  reserve tiles
	width = surface->w / TILE_SIZE, height = surface->h / TILE_SIZE;
	tiles.resize( (size_t) ( surface->w * surface->h ), false );

	int dots_count { 0 };

	//  access image pixels
	SDL_LockSurface( surface );
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
						if ( pixel == DOT_COLOR_PIXEL )
						{
							if ( x == 0 && y == 0 )
							{
								Vec2 pos = {
									(float) ( tx * TILE_SIZE ),
									(float) ( ty * TILE_SIZE )
								};

								Entity* ent { nullptr };
								if ( Texture::get_pixel_at( surface, tx * TILE_SIZE + x + 1, ty * TILE_SIZE + y + 1 ) == DOT_COLOR_PIXEL )
								{
									ent = new PowerPellet();
								}
								else
								{
									ent = new PacDot();
								}

								ent->transform->pos = pos;

								//  remove pixels from image
								SDL_Rect rect {
									(int) pos.x,
									(int) pos.y,
									TILE_SIZE,
									TILE_SIZE
								};
								SDL_FillRect( surface, &rect, Color::transparent.to_pixel() );

								//  increase dot count
								dots_count++;

								//  skip to next tile
								is_breaking = true;
								break;
							}
						}

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
	SDL_UnlockSurface( surface );

	//  set dots count
	auto& stats = GameStats::instance();
	stats.remaining_dots = dots_count;

	//  gen normal texture
	normal_texture = Texture::load_from_surface( game->get_render_batch(), TEXTURE_PATH, surface, false );
	gen_blink_texture( surface );

	//  setup sprite renderer
	sprite->texture = normal_texture;
	sprite->size_to_texture();
}

void Level::gen_blink_texture( SDL_Surface* surface )
{
	SDL_LockSurface( surface );
	for ( int x = 0; x < surface->w; x++ )
	{
		for ( int y = 0; y < surface->h; y++ )
		{
			Color color;

			//  get replace color
			uint32_t pixel = Texture::get_pixel_at( surface, x, y );
			if ( pixel == WALL_COLOR_PIXEL )
			{
				color = Color::white;
			}
			else
			{
				color = Color::transparent;
			}

			//  change color
			SDL_Rect rect { x, y, 1, 1 };
			SDL_FillRect( surface, &rect, color.to_pixel() );
		}
	}
	SDL_UnlockSurface( surface );

	blink_texture = Texture::load_from_surface( game->get_render_batch(), TEXTURE_PATH, surface );
}