#include "timer.h"

#include <SDL_timer.h>
#include <algorithm>

using namespace suprengine;

unsigned int Timer::compute_dt()
{
	_frame_start = SDL_GetTicks();
	uint32_t dt = _frame_start - _last_frame;
	_last_frame = _frame_start;

	return std::min( dt, MAX_DT );
}

void Timer::delay_time()
{
	_frame_time = SDL_GetTicks() - _frame_start;
	if ( _frame_time < FRAME_DELAY )
	{
		SDL_Delay( FRAME_DELAY - _frame_time );
	}
}