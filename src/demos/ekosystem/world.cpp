#include "world.h"

#include <suprengine/assets.h>
#include <suprengine/components/colliders/box-collider.h>
#include <suprengine/random.h>

#include "entities/pawn.h"

using namespace eks;

World::World( const Vec2& size )
	: _size( size )
{
	auto& engine = Engine::instance();
	auto model = Assets::get_model( MESH_CUBE );

	//  setup ground
	auto ground = engine.create_entity<Entity>();
	ground->transform->location = Vec3 {
		_size.x * TILE_SIZE * 0.5f,
		_size.y * TILE_SIZE * 0.5f,
		-TILE_SIZE * 0.5f
	};
	ground->transform->scale = Vec3 {
		_size.x * TILE_SIZE * 0.5f,
		_size.y * TILE_SIZE * 0.5f,
		1.0f
	};
	ground->create_component<ModelRenderer>( model, SHADER_LIT_MESH );
	ground->create_component<BoxCollider>( Box::HALF );
	_wk_ground = ground;
}

World::~World()
{
	clear();

	if ( auto ground = _wk_ground.lock() )
	{
		ground->kill();
	}
}

shared_ptr<Pawn> World::create_pawn( const Vec3& tile_pos )
{
	auto& engine = Engine::instance();
	auto model = Assets::get_model( MESH_CUBE );

	auto pawn = engine.create_entity<Pawn>( this, model );
	pawn->set_tile_pos( tile_pos );

	_wk_pawns.push_back( pawn );
	return pawn;
}

void World::clear()
{
	for ( auto& wk_pawn : _wk_pawns ) 
	{
		if ( auto pawn = wk_pawn.lock() )
		{
			pawn->kill();
		}
	}
	_wk_pawns.clear();
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
