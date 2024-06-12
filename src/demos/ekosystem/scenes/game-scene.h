#pragma once

#include <suprengine/scene.h>
#include <suprengine/components/colliders/box-collider.h>

#include "entities/pawn.h"

#include "components/camera-controller.h"

#include "debug-menu.h"

using namespace suprengine;

namespace eks
{
	class GameScene : public Scene
	{
	public:
		GameScene();
		virtual ~GameScene();

		void init() override;

		void setup_world();

		void update( float dt ) override;

	private:
		World* _world { nullptr };
		SafePtr<CameraController> _camera_controller;
		DebugMenu _debug_menu;
	};
}