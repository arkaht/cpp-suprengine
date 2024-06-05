#pragma once
#include "sprite-renderer.hpp"

namespace suprengine
{
	struct AnimClip
	{
		int start_frame { 0 }, end_frame { 0 };
		float time_per_frame { 0.0f };
	};

	class AnimSpriteRenderer : public SpriteRenderer
	{
	private:
		float current_time_per_frame { 0.0f };
	public:
		std::map<std::string, AnimClip> clips;
		std::string current_clip { "" };

		std::vector<Rect> frames;
		int current_frame { 0 };
		float time_per_frame { 0.0f };
		bool is_looping { true };
		bool is_playing { true };

		AnimSpriteRenderer( shared_ptr<Entity> owner, Texture* texture, int fps = 6, int priority_order = 0 )
			: SpriteRenderer( owner, texture, priority_order ) 
		{
			set_fps( fps );
		}
	
		void update( float dt ) override
		{
			if ( !is_playing ) return;


			if ( ( current_time_per_frame += dt ) >= time_per_frame )
			{
				current_time_per_frame -= time_per_frame;

				int start_frame { 0 }, end_frame = (int) frames.size();

				//  retrieve anim clip infos
				if ( current_clip != "" )
				{
					AnimClip& clip = clips[current_clip];
					start_frame = clip.start_frame, end_frame = clip.end_frame + 1;
				}

				//  increment current frame
				int next_frame = current_frame + 1;
				if ( next_frame >= end_frame )
				{
					if ( is_looping )
					{
						set_current_frame( start_frame );
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
		
			//  set as current frame
			if ( frames.size() == 1 )
			{
				set_current_frame( 0 );
			}
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

		void set_current_clip( const std::string& clip_name )
		{
			if ( current_clip == clip_name ) return;

			if ( clips.find( clip_name ) == clips.end() )
			{
				Logger::error( "AnimSpriteRenderer: clip '" + clip_name + "' doesn't exists, aborting!" );
				return;
			};

			current_clip = clip_name;

			//  update from clip
			AnimClip& clip = clips[clip_name];
			if ( clip.time_per_frame > 0.0f )
			{
				time_per_frame = clip.time_per_frame;
			}
			set_current_frame( clip.start_frame );
		}

		void add_clip( const std::string& name, const AnimClip& clip )
		{
			clips[name] = clip;
		}
	};
}