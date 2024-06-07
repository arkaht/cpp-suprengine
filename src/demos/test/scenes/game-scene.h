#pragma once

#include <suprengine/scene.h>
#include <suprengine/components/colliders/box-collider.h>
#include <suprengine/components/mover.hpp>
#include <suprengine/components/mouse-looker.hpp>
#include <suprengine/components/renderers/model-renderer.hpp>

#include "camera-demo.hpp"

using namespace suprengine;

namespace test
{
	class GameScene : public Scene
	{
	public:
		void init() override;

		void update( float dt ) override;

	private:
		SharedPtr<ModelRenderer> model_renderer;
	};
}