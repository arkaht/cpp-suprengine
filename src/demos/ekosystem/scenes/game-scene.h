#pragma once

#include <suprengine/scene.h>
#include <suprengine/components/colliders/box-collider.h>

#include "entities/pawn.h"

#include "components/camera-controller.h"

using namespace suprengine;

namespace eks
{
	class GameScene : public Scene
	{
	public:
		virtual ~GameScene();

		void init() override;

		void update( float dt ) override;

	private:
		World* _world { nullptr };
		Pawn* _test_pawn { nullptr };
		ref<CameraController> _camera_controller { nullptr };
	};
}