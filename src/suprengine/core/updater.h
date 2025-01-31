#pragma once

#include <suprengine/utils/usings.h>

namespace suprengine
{
	class Updater
	{
	public:
		Updater() {};
		Updater( const Updater& ) = delete;
		Updater& operator=( const Updater& ) = delete;

		void compute_delta_time();
		float get_scaled_delta_time() const;
		float get_unscaled_delta_time() const;

		/*
		 * Delays main thread to the target frame rate.
		 * If 'is_fps_capped' set to false, this function will result in no operation.
		 */
		void delay_time();

		void accumulate_seconds( float dt );
		float get_accumulated_seconds() const;
		uint32 get_fps() const;

		uint32 get_frame_tick() const;

	public:
		float time_scale = 1.0f;
		int target_fps = 60;
		bool is_fps_capped = true;

	private:
		uint32 _frame_start = 0u;
		uint32 _last_frame = 0u;
		uint32 _fps = 0;

		float _accumulated_seconds = 0.0f;
		float _delta_time = 0.0f;
	};
}