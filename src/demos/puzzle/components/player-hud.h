#pragma once
#include <suprengine/components/renderer.h>

namespace puzzle
{
	using namespace suprengine;

	class PlayerHUD : public Renderer
	{
	public:
		PlayerHUD( Entity* owner );

		void render() override;

	private:
		Texture* _crosshair_line_texture;
	};
}