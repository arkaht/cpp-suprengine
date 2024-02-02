#include "player-hud.h"

#include <suprengine/easing.h>

#include <entities/player-spaceship-controller.h>

using namespace puzzle;

PlayerHUD::PlayerHUD( 
	PlayerSpaceshipController* owner
)
	: _controller( owner ),
	  Renderer( owner )
{
	_controller->event_on_possess_changed.listen( "player-hud",
		[&]( Spaceship* previous, Spaceship* current )
		{
			_unbind_from_spaceship( previous );
			_bind_to_spaceship( current );
		}
	);

	_crosshair_line_texture = Assets::get_texture( "crosshair-line" );
	_kill_icon_texture = Assets::get_texture( "kill-icon" );
}

void PlayerHUD::update( float dt )
{
	auto spaceship = _controller->get_ship();
	if ( !spaceship ) return;

	_hit_time = math::max( 0.0f, _hit_time - dt );
	_crosshair_color = Color::lerp( 
		_crosshair_color, 
		spaceship->get_color(), 
		dt * CROSSHAIR_COLOR_SMOOTH_SPEED 
	);

	if ( _kill_time > 0.0f )
	{
		_kill_time = math::max( 0.0f, _kill_time - dt );
		_kill_color = Color::lerp(
			_kill_color,
			_target_kill_color,
			dt * KILL_COLOR_IN_SPEED
		);
		_kill_scale = easing::out_expo( 
			math::min( KILL_SCALE_TIME, KILL_TIME - _kill_time ) / KILL_SCALE_TIME );
	}
	else
	{
		_kill_color = Color::lerp( 
			_kill_color, 
			Color::transparent,
			dt * KILL_COLOR_OUT_SPEED
		);
	}
}

void PlayerHUD::render()
{
	auto game = owner->get_engine();
	auto window = game->get_window();
	auto camera = game->camera;

	auto spaceship = _controller->get_ship();
	if ( !spaceship ) return;

	Vec2 window_size = window->get_size();

	//  crosshair
	{
		Vec3 aim_location = spaceship->get_shoot_location( Vec3 { 1.0f, 0.0f, 1.0f } );
		aim_location += spaceship->transform->get_forward() * CROSSHAIR_DISTANCE;

		Vec3 crosshair_pos = camera->world_to_viewport( aim_location );
		if ( crosshair_pos.z > 0.0f )
		{
			_draw_crosshair( crosshair_pos );
		}
	}

	/*if ( _kill_time > 0.0f )
	{*/
		_render_batch->draw_texture( 
			Vec2 { 
				window_size.x * 0.5f,
				window_size.y * 0.25f,
			},
			Vec2::one * _kill_scale * 0.1f,
			0.0f,
			Vec2 { 0.5f, 0.5f },
			_kill_icon_texture,
			_kill_color
		);
	/*}*/
}

void PlayerHUD::_bind_to_spaceship( Spaceship* spaceship )
{
	if ( !spaceship ) return;

	//  color
	_crosshair_color = spaceship->get_color();

	//  events
	spaceship->on_hit.listen( "player-hud", 
		std::bind( &PlayerHUD::_on_spaceship_hit, this, std::placeholders::_1 ) );
}

void PlayerHUD::_unbind_from_spaceship( Spaceship* spaceship )
{
	if ( !spaceship ) return;

	spaceship->on_hit.unlisten( "player-hud" );
}

void PlayerHUD::_draw_crosshair( const Vec2& pos )
{
	auto spaceship = _controller->get_ship();

	const float angle_iter = math::DOUBLE_PI / CROSSHAIR_LINES_COUNT;
	
	float shoot_ratio = 
		easing::in_out_cubic( spaceship->get_shoot_time() / 0.15f );
	float hit_ratio =
		easing::in_out_cubic( _hit_time / HIT_TIME );

	float distance = CROSSHAIR_LINES_DISTANCE 
		+ CROSSHAIR_LINES_SHOOT_DISTANCE * shoot_ratio;
	Vec2 scale = CROSSHAIR_LINE_SCALE
		+ CROSSHAIR_LINES_SHOOT_SCALE * hit_ratio;

	float angle = CROSSHAIR_START_ANGLE;
	for ( int i = 0; i < CROSSHAIR_LINES_COUNT; i++ )
	{
		Vec2 offset {
			math::cos( angle ) * distance,
			math::sin( angle ) * distance,
		};

		_render_batch->draw_texture( 
			pos + offset,
			scale,
			angle,
			Vec2 { 0.5f, 0.5f }, 
			_crosshair_line_texture,
			_crosshair_color
		);
		angle += angle_iter;
	}
}

void PlayerHUD::_on_spaceship_hit( const DamageResult& result )
{
	_hit_time = HIT_TIME;
	_crosshair_color = Color::white;

	auto health = result.victim.lock();
	if ( !health ) return;

	auto spaceship = dynamic_cast<Spaceship*>( health->get_owner() );
	if ( spaceship && !result.is_alive )
	{
		_kill_time = KILL_TIME;
		_kill_color = Color::transparent;
		_target_kill_color = spaceship->get_color();
	}
}
