#pragma once

namespace suprengine
{
	constexpr unsigned int FPS = 60;
	constexpr unsigned int FRAME_DELAY = 1000 / FPS;
	constexpr unsigned int MAX_DT = 50;

	class Updater
	{
	private:
		unsigned int frame_start { 0u }, last_frame { 0u }, frame_time { 0u };
		float accumulated_seconds { 0.0f };
	public:
		Updater() {};
		Updater( const Updater& ) = delete;
		Updater& operator=( const Updater& ) = delete;

		unsigned int compute_dt();

		void delay_time();

		void accumulate_seconds( float dt ) { accumulated_seconds += dt; }
		float get_accumulated_seconds() const { return accumulated_seconds; }
	};
}