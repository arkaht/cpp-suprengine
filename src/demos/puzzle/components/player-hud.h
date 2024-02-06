#pragma once
#include <suprengine/components/renderer.h>

#include <components/health-component.h>

namespace puzzle
{
	using namespace suprengine;

	class Spaceship;
	class PlayerSpaceshipController;

	struct KillIconData
	{
		float x_offset = -1.0f;

		float life_time { 0.0f };
		float scale { 1.0f };

		Color color { Color::transparent };
		Color target_color { Color::white };
	};

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
		const float KILL_SCALE_TIME = 0.16f;
		const float KILL_COLOR_IN_SPEED = 4.0f;
		const float KILL_ALPHA_IN_SPEED = 12.0f;
		const float KILL_COLOR_OUT_SPEED = 6.0f;
		const float KILL_ICON_SIZE = 48.0f;
		float KILL_ICON_TEXTURE_SCALE = -1.0f;  //  auto-filled
		const float KILL_X_GAP = 8.0f;
		const float KILL_X_SPEED = 6.0f;

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
		std::vector<KillIconData> _kill_icons;
		float _hit_time = 0.0f;

		PlayerSpaceshipController* _controller;
	};
}