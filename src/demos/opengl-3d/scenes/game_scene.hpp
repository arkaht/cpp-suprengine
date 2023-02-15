#pragma once

#include <suprengine/scene.h>
#include <suprengine/ecs/components/renderers/rect_renderer.hpp>

using namespace suprengine;

namespace demo_opengl3d
{
	class GameScene : public Scene
	{
	public:
		GameScene() {}

		void init() override 
		{
			printf( "\n" );

			auto ent = new Entity();
			( new RectRenderer( ent, Rect { 0.0f, 0.0f, 256.0f, 128.0f } ) )->modulate = Color::red;
			( new RectRenderer( ent, Rect { 1024.0f, 512.0f, 128.0f, 512.0f } ) )->modulate = Color::green;
			( new RectRenderer( ent, Rect { 512.0f, 128.0f, 128.0f, 256.0f } ) )->modulate = Color::blue;
		}
	};
}