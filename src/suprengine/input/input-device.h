#pragma once

namespace suprengine
{
	class InputManager;

	class InputDevice
	{
	public:
		InputDevice(InputManager* manager)
			: _manager(manager) {}
		virtual ~InputDevice() = default;

		virtual void update() = 0;

		virtual void populate_imgui() {};

	protected:
		InputManager* _manager;
	};
}