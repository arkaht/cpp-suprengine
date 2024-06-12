#include "updater.h"

#include <SDL_timer.h>
#include <algorithm>

using namespace suprengine;

void Updater::compute_delta_time()
{
	_frame_start = SDL_GetTicks();
	uint32_t dt = _frame_start - _last_frame;
	_last_frame = _frame_start;

	_delta_time = std::min( dt, MAX_DT ) / 1000.0f;
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
	_frame_time = SDL_GetTicks() - _frame_start;
	if ( _frame_time < FRAME_DELAY )
	{
		SDL_Delay( FRAME_DELAY - _frame_time );
	}
}

void Updater::accumulate_seconds( float dt ) 
{ 
	_accumulated_seconds += dt;
}

float Updater::get_accumulated_seconds() const 
{ 
	return _accumulated_seconds; 
}
