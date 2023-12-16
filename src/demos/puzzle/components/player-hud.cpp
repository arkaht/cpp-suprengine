#include "player-hud.h"

using namespace puzzle;

PlayerHUD::PlayerHUD( Entity* owner )
	: Renderer( owner )
{
	_crosshair_line_texture = Assets::get_texture( "crosshair-line" );
}

void PlayerHUD::render()
{
	auto window = owner->get_game()->get_window();
	Vec2 window_size = window->get_size();

	Rect src { Vec2::zero, _crosshair_line_texture->get_size() };
	Rect dst { window_size / 2.0f, src.get_size() };
	_render_batch->draw_texture( src, dst, 0.0f, src.get_size(), _crosshair_line_texture );
}
