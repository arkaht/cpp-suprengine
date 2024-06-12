#pragma once

#include <suprengine/engine.h>

#include <world.h>
#include <entities/pawn.h>
#include <components/camera-controller.h>

namespace eks
{
	using namespace suprengine;

	class DebugMenu
	{
	public:
		DebugMenu();
		~DebugMenu();

		void populate();

	public:
		World* world;
		SafePtr<CameraController> camera_controller;
	};
}