#include "player-hud.h"

using namespace puzzle;

PlayerHUD::PlayerHUD( Entity* owner, Color color )
	: _color( color ),
	  Renderer( owner )
{
	_crosshair_line_texture = Assets::get_texture( "crosshair-line" );
}

void PlayerHUD::render()
{
	auto game = owner->get_game();
	auto window = game->get_window();
	auto camera = game->camera;

	Vec2 window_size = window->get_size();

	//  crosshair
	Vec3 aim_location = owner->transform->location 
		+ owner->transform->get_forward() * CROSSHAIR_DISTANCE;
	Vec3 crosshair_pos = camera->world_to_viewport( aim_location );
	if ( crosshair_pos.z > 0.0f )
	{
		draw_crosshair( crosshair_pos );
	}
}

void PlayerHUD::draw_crosshair( const Vec2& pos )
{
	const float angle_iter = math::DOUBLE_PI / CROSSHAIR_LINES_COUNT;
	
	float angle = CROSSHAIR_START_ANGLE;
	for ( int i = 0; i < CROSSHAIR_LINES_COUNT; i++ )
	{
		Vec2 offset {
			math::cos( angle ) * CROSSHAIR_LINES_DISTANCE,
			math::sin( angle ) * CROSSHAIR_LINES_DISTANCE,
		};

		_render_batch->draw_texture( 
			pos + offset,
			CROSSHAIR_LINE_SCALE,
			angle,
			Vec2 { 0.5f, 0.5f }, 
			_crosshair_line_texture,
			_color
		);
		angle += angle_iter;
	}
}
