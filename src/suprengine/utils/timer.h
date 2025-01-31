#pragma once

#include <functional>

#define TIMER( time, code )  Engine::instance().add_timer( Timer( [&]() { code }, time ) )

namespace suprengine
{
	using lambda = std::function<void()>;
	struct Timer
	{
		Timer() {}
		Timer( const lambda& callback, float time, std::uint32_t repetitions = 1 )
			: max_time( time ), repetitions( repetitions ), callback( callback )
		{}

		/*
		 * Maximum time in seconds the timer should wait before calling
		 * the callback.
		 */
		float max_time = 1.0f;
		/*
		 * Number of times the timer should run.
		 * Set to 0 for infinite looping.
		 */
		std::uint32_t repetitions = 1;

		/*
		 * Timer's function to call once the time is depleted.
		 */
		lambda callback;

		/*
		 * Timer's current time.
		 * Once it exceeded the maximum time, the callback is called.
		 */
		float current_time = 0.0f;
	};
}