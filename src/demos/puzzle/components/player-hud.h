#pragma once
#include <suprengine/components/renderer.h>

#include <components/health-component.h>

namespace puzzle
{
	using namespace suprengine;

	class Spaceship;
	class PlayerSpaceshipController;

	class PlayerHUD : public Renderer
	{
	public:
		PlayerHUD( 
			PlayerSpaceshipController* owner
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

		const float KILL_TIME = 2.5f;
		const float KILL_SCALE_TIME = 0.33f;
		const float KILL_COLOR_IN_SPEED = 8.0f;
		const float KILL_COLOR_OUT_SPEED = 6.0f;

		const float HIT_TIME = 0.25f;

	private:
		void _bind_to_spaceship( Spaceship* spaceship );
		void _unbind_from_spaceship( Spaceship* spaceship );

		void _draw_crosshair( const Vec2& pos );

		void _on_spaceship_hit( const DamageResult& result );

	private:
		Texture* _crosshair_line_texture;
		Texture* _kill_icon_texture;

		Color _crosshair_color = Color::white;
		Color _kill_color = Color::transparent;
		Color _target_kill_color = Color::white;
		float _kill_scale = 1.0f;
		float _hit_time = 0.0f;
		float _kill_time = 0.0f;

		PlayerSpaceshipController* _controller;
	};
}