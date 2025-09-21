#include "rect.h"

#include "suprengine/utils/assert.h"

using namespace suprengine;

Rect Rect::split( const SplitDirection direction, const float ratio )
{
	switch ( direction )
	{
		case SplitDirection::Horizontal:
		{
			const float old_width = w;
			w *= ratio;
			return Rect( x + w, y, old_width * ( 1.0f - ratio ), h );
		}
		case SplitDirection::Vertical:
		{
			const float old_height = h;
			h *= ratio;
			return Rect( x, y + h, w, old_height * ( 1.0f - ratio ) );
		}
		default:
			ASSERT( false );
			break;
	}

	return Rect {};
}

Vec2 Rect::get_pos() const
{
	return Vec2 { x, y };
}

Vec2 Rect::get_size() const
{
	return Vec2 { w, h };
}

SDL_Rect Rect::to_sdl_rect() const
{
	return SDL_Rect{
		static_cast<int>( x ),
		static_cast<int>( y ),
		static_cast<int>( w ),
		static_cast<int>( h ),
	};
}

void Rect::add_pos( const Vec2& pos )
{
	x += pos.x, y += pos.y;
}

void Rect::set_pos( const Vec2& pos )
{
	x = pos.x, y = pos.y;
}

void Rect::set_size( const Vec2& size )
{
	w = size.x, h = size.y;
}

std::string Rect::to_string() const
{
	return "x=" + std::to_string( x ) + ";y=" + std::to_string( y ) + ";"
	       + "w=" + std::to_string( w ) + ";h=" + std::to_string( h );
}
