#pragma once
#include <suprengine/components/renderer.h>

namespace puzzle
{
	using namespace suprengine;

	class PlayerSpaceship;

	class PlayerHUD : public Renderer
	{
	public:
		PlayerHUD( 
			Entity* owner, 
			PlayerSpaceship* spaceship, 
			Color color 
		);

		void update( float dt ) override;
		void render() override;

		RenderPhase get_render_phase() const override 
		{
			return RenderPhase::Viewport;
		}

	private:
		const float CROSSHAIR_DISTANCE = 500.0f;
		const float CROSSHAIR_START_ANGLE = math::HALF_PI;
		const float CROSSHAIR_LINES_COUNT = 3;
		const float CROSSHAIR_LINES_DISTANCE = 14.0f;
		const float CROSSHAIR_LINES_SHOOT_DISTANCE = 3.0f;
		const Vec2  CROSSHAIR_LINES_SHOOT_SCALE { 0.25f, 0.85f };
		const Vec2  CROSSHAIR_LINE_SCALE { 0.5f, 0.75f };
		const float CROSSHAIR_COLOR_SMOOTH_SPEED = 4.0f;

		const float HIT_TIME = 0.25f;

	private:
		void draw_crosshair( const Vec2& pos );

	private:
		Texture* _crosshair_line_texture;
		Color _color = Color::white;

		Color _crosshair_color;
		float _hit_time = 0.0f;

		PlayerSpaceship* _spaceship;
	};
}