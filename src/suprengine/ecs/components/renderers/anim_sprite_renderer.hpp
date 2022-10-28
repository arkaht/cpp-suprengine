#pragma once
#include "sprite_renderer.hpp"

namespace suprengine
{
	class AnimSpriteRenderer : public SpriteRenderer
	{
	private:
		float current_time_per_frame { 0.0f };
	public:
		std::vector<Rect> frames;
		int current_frame { 0 };
		float time_per_frame { 0.0f };
		bool is_looping { true };
		bool is_playing { true };

		AnimSpriteRenderer( Entity* owner, Texture* texture, int fps = 6, Color modulate = Color::white, int priority_order = 0 )
			: SpriteRenderer( owner, texture, modulate, priority_order ) 
		{
			set_fps( fps );
		}
	
		void update( float dt ) override
		{
			if ( !is_playing ) return;

			if ( ( current_time_per_frame += dt ) >= time_per_frame )
			{
				current_time_per_frame -= time_per_frame;

				//  increment current frame
				int next_frame = current_frame + 1;
				if ( next_frame >= frames.size() )
				{
					if ( is_looping )
					{
						set_current_frame( 0 );
					}
					else
					{
						is_playing = false;
					}
				}
				else
				{
					set_current_frame( next_frame );
				}
			}
		}

		void set_fps( int fps )
		{
			time_per_frame = 1.0f / fps;
		}

		void set_current_frame( int frame )
		{
			current_frame = frame;
			source = frames[current_frame];
		}

		void add_frame( const Rect& rect )
		{
			frames.push_back( rect );
		}

		void gen_frames( const Rect& region, const Vec2& size )
		{
			for ( float y = region.y; y < region.y + region.h; y += size.y )
			{
				for ( float x = region.x; x < region.x + region.w; x += size.x )
				{
					add_frame( Rect { x, y, size.x, size.y } );
				}
			}
		}
	};
}