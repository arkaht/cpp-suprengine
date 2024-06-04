#include "world.h"

#include <suprengine/assets.h>
#include <suprengine/components/colliders/box-collider.h>
#include <suprengine/random.h>

#include "entities/pawn.h"

using namespace eks;

World::World( const Vec2& size )
	: _size( size )
{
	auto model = Assets::get_model( MESH_CUBE );

	//  setup ground
	_ground = new Entity();
	_ground->transform->location = Vec3 {
		_size.x * TILE_SIZE * 0.5f,
		_size.y * TILE_SIZE * 0.5f,
		-TILE_SIZE * 0.5f
	};
	_ground->transform->scale = Vec3 {
		_size.x * TILE_SIZE * 0.5f,
		_size.y * TILE_SIZE * 0.5f,
		1.0f
	};
	_ground->create_component<ModelRenderer>( model, SHADER_LIT_MESH );
	_ground->create_component<BoxCollider>( Box::HALF );
}

World::~World()
{
	for ( Pawn* pawn : _pawns )
	{
		delete pawn;
	}

	if ( _ground != nullptr )
	{
		delete _ground;
	}
}

Pawn* World::create_pawn( const Vec3& tile_pos )
{
	auto model = Assets::get_model( MESH_CUBE );

	auto pawn = new Pawn( this, model );
	pawn->set_tile_pos( tile_pos );

	_pawns.push_back( pawn );
	return pawn;
}

Vec3 World::find_random_tile_pos()
{
	return Vec3::snap_to_grid(
		random::generate_location(
			0.0f, 0.0f, 0.0f,
			_size.x, _size.y, 0.0f
		),
		1.0f
	);
}

Vec2 World::get_size() const
{ 
	return _size; 
}
