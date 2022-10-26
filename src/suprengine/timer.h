#pragma once

namespace suprengine
{
	constexpr unsigned int FPS = 60;
	constexpr unsigned int FRAME_DELAY = 1000 / FPS;
	constexpr unsigned int MAX_DT = 50;

	class Timer
	{
	private:
		unsigned int _frame_start { 0u }, _last_frame { 0u }, _frame_time { 0u };
		float _accumulated_seconds { 0.0f };
	public:
		Timer() {};
		Timer( const Timer& ) = delete;
		Timer& operator=( const Timer& ) = delete;

		unsigned int compute_dt();

		void delay_time();

		void accumulate_seconds( float dt ) { _accumulated_seconds += dt; }
		float get_accumulated_seconds() const { return _accumulated_seconds; }
	};
}