#include "pawn.h"

using namespace eks;

Pawn::Pawn( const ref<Model>& model )
{
	_renderer = create_component<ModelRenderer>( model );
}

void Pawn::update_this( float dt )
{
	if ( _is_moving )
	{
		transform->set_location( 
			Vec3::approach( 
				transform->location,
				_move_target,
				TILE_SIZE * MOVE_SPEED * dt
			) 
		);

		float dist = Vec3::distance( transform->location, _move_target );
		if ( math::near_zero( dist ) )
		{
			_is_moving = false;
		}
	}
}

void Pawn::move_to( const Vec3& target )
{
	_move_target = transform->location + 
		( target - transform->location ) * TILE_SIZE;
	_is_moving = true;
}
