#pragma once
#include <suprengine/components/renderer.h>

namespace puzzle
{
	using namespace suprengine;

	class PlayerHUD : public Renderer
	{
	public:
		PlayerHUD( Entity* owner, Color color );

		void render() override;

		RenderPhase get_render_phase() const override 
		{
			return RenderPhase::SPRITE;
		}

	private:
		const float CROSSHAIR_DISTANCE = 500.0f;
		const float CROSSHAIR_START_ANGLE = math::HALF_PI;
		const float CROSSHAIR_LINES_COUNT = 3;
		const float CROSSHAIR_LINES_DISTANCE = 14.0f;
		const Vec2  CROSSHAIR_LINE_SCALE { 0.5f, 0.5f };

	private:
		void draw_crosshair( const Vec2& pos );

	private:
		Texture* _crosshair_line_texture;
		Color _color = Color::white;
	};
}