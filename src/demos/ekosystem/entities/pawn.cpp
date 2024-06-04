#include "pawn.h"

using namespace eks;

Pawn::Pawn( const ref<Model>& model )
{
	_renderer = create_component<ModelRenderer>( model, SHADER_LIT_MESH, Color::blue );
}

void Pawn::update_this( float dt )
{
	if ( _move_progress < 1.0f )
	{
		_move_progress = math::min( 
			_move_progress + MOVE_SPEED * dt, 
			1.0f 
		);

		Vec3 new_tile_pos = Vec3::lerp( 
			_move_from,
			_move_to,
			_move_progress
		);
		if ( _move_progress >= 1.0f )
		{
			_tile_pos = new_tile_pos;
		}
		//printf( "%s\n", new_tile_pos.to_string().c_str() );

		transform->set_location( new_tile_pos * TILE_SIZE );
	}
}

void Pawn::move_to( const Vec3& target )
{
	_move_from = _move_to;
	_move_to = target;
	_move_progress = 0.0f;
}

void Pawn::update_tile_pos()
{
	_tile_pos = Vec3::snap_to_grid( transform->location, TILE_SIZE );
	_move_to = _tile_pos;
}

Vec3 Pawn::get_tile_pos() const
{
	return _tile_pos;
}
