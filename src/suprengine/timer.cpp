#include "timer.h"

#include <SDL_timer.h>
#include <algorithm>

using namespace suprengine;

unsigned int Timer::compute_dt()
{
	frame_start = SDL_GetTicks();
	uint32_t dt = frame_start - last_frame;
	last_frame = frame_start;

	return std::min( dt, MAX_DT );
}

void Timer::delay_time()
{
	frame_time = SDL_GetTicks() - frame_start;
	if ( frame_time < FRAME_DELAY )
	{
		SDL_Delay( FRAME_DELAY - frame_time );
	}
}