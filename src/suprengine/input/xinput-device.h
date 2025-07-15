#pragma once

#ifdef PLATFORM_WINDOWS

// Link XInput 1.4 library
#ifdef _MSC_VER
#pragma comment( lib, "xinput" )
#endif

// Exclude rarely-used stuff from Windows headers
#define WIN32_LEAN_AND_MEAN
// Avoid conflicts with min and max methods with std::numeric_limits
#define NOMINMAX

// Forced to include "windows.h" to avoid a "No Target Architecture" pre-processor
// error when including "Xinput.h"
#include <windows.h>

#undef NOMINMAX
#undef WIN32_LEAN_AND_MEAN

#include <Xinput.h>

#else

struct XINPUT_STATE {};
#define XUSER_MAX_COUNT 4

#endif

#include "input-device.h"

#include "suprengine/utils/usings.h"

namespace suprengine
{
	struct XInputGamepadState
	{
		int gamepad_id = INDEX_NONE;
		float next_update_time = 0.0f;

		XINPUT_STATE internal_state;
	};

	class XInputDevice : public InputDevice
	{
	public:
		explicit XInputDevice( InputManager* manager );

		void update() override;

		void populate_imgui() override;

	private:
		XInputGamepadState _gamepads[XUSER_MAX_COUNT] {};
	};
}
