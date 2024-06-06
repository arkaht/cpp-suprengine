#pragma once

#include <functional>

#define TIMER( time, code )  Engine::instance().add_timer( { time, [&]() code } )

namespace suprengine
{
	using lambda = std::function<void()>;
	struct Timer
	{
		float time { 0.0f };
		lambda callback;
	};
}