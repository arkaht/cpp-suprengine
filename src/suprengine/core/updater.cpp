#include "updater.h"
#include <suprengine/math/math.h>

#include <SDL_timer.h>

using namespace suprengine;

constexpr uint32 MAX_DT = 50;

void Updater::compute_delta_time()
{
	_frame_start = SDL_GetTicks();
	uint32_t dt = _frame_start - _last_frame;
	_last_frame = _frame_start;

	_delta_time = math::min( dt, MAX_DT ) / 1000.0f;
}

float Updater::get_scaled_delta_time() const
{
	return _delta_time * time_scale;
}

float Updater::get_unscaled_delta_time() const
{
	return _delta_time;
}

void Updater::delay_time()
{
	if ( !is_fps_capped ) return;

	uint32 target_frame_time = static_cast<uint32>( 1.0f / target_fps * 1000.0f );
	uint32 frame_time = SDL_GetTicks() - _frame_start;
	if ( frame_time < target_frame_time )
	{
		SDL_Delay( target_frame_time - frame_time );
	}
}

void Updater::accumulate_seconds( float dt ) 
{ 
	float last_seconds = _accumulated_seconds;
	_accumulated_seconds += dt;

	//  NOTE: FPS counter is dependent on time scale, which shouldn't be the case!
	//  TODO: Don't use _accumulated_seconds since it's time scale dependent 
	if ( (int)math::floor( last_seconds ) != (int)math::floor( _accumulated_seconds ) )
	{
		_fps = (uint32_t)math::floor( 1.0f / get_unscaled_delta_time() );
	}
}

float Updater::get_accumulated_seconds() const 
{ 
	return _accumulated_seconds; 
}

uint32_t Updater::get_fps() const
{
	return _fps;
}

uint32_t Updater::get_frame_tick() const
{
	return _frame_start;
}
