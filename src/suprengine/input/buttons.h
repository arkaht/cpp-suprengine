#pragma once

#include <suprengine/utils/enum-flags.h>
#include <suprengine/utils/usings.h>

namespace suprengine
{
	enum class KeyState : uint8
	{
		Up		 = 0,
		Down	 = 1,
		Pressed	 = 2,
		Released = 3,
	};

	enum class MouseButton : uint8
	{
		None   = 0,
		Left   = 1 << 0,
		Middle = 1 << 1,
		Right  = 1 << 2,
	};
	DEFINE_ENUM_WITH_FLAGS( MouseButton, uint8 )

	/*
	 * Enum flags, holding all generic gamepad buttons.
	 * Values are purposely mapped to XInput buttons values for a better integration.
	 */
	enum class GamepadButton : uint16
	{
		None			= 0,
		DpadUp			= 1 << 0,
		DpadDown		= 1 << 1,
		DpadLeft		= 1 << 2,
		DpadRight		= 1 << 3,
		Start			= 1 << 4,
		Back			= 1 << 5,
		LeftThumb		= 1 << 6,
		RightThumb		= 1 << 7,
		LeftShoulder	= 1 << 8,
		RightShoulder	= 1 << 9,

		// XInput does not implement the trigger values as buttons.
		// Fortunately, the mapping of further values is still correct because it jumped over them.

		LeftTrigger		= 1 << 10,
		RightTrigger	= 1 << 11,

		FaceButtonDown	= 1 << 12,
		FaceButtonRight = 1 << 13,
		FaceButtonLeft	= 1 << 14,
		FaceButtonUp	= 1 << 15,
	};
	DEFINE_ENUM_WITH_FLAGS( GamepadButton, uint16 )

	static MouseButton sdl_to_mouse_button( const uint8 button_index )
	{
		switch ( button_index )
		{
			case 1:
				return MouseButton::Left;
			case 2:
				return MouseButton::Middle;
			case 3:
				return MouseButton::Right;
		}

		return MouseButton::None;
	}
}