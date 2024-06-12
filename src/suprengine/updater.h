#pragma once

#include <cstdint>

namespace suprengine
{
	constexpr uint32_t FPS = 60;
	constexpr uint32_t FRAME_DELAY = 1000u / FPS;
	constexpr uint32_t MAX_DT = 50;

	class Updater
	{
	public:
		Updater() {};
		Updater( const Updater& ) = delete;
		Updater& operator=( const Updater& ) = delete;

		void compute_delta_time();
		float get_scaled_delta_time() const;
		float get_unscaled_delta_time() const;

		void delay_time();

		void accumulate_seconds( float dt );
		float get_accumulated_seconds() const;

	public:
		float time_scale = 1.0f;

	private:
		uint32_t _frame_start = 0u;
		uint32_t _last_frame = 0u;
		uint32_t _frame_time = 0u;

		float _accumulated_seconds = 0.0f;
		float _delta_time = 0.0f;
	};
}