#pragma once

#include <suprengine/entity.h>

namespace eks
{
	using namespace suprengine;

	class Pawn;

	class World 
	{
	public:
		World( const Vec2& size );
		~World();

		Pawn* create_pawn( const Vec3& tile_pos );

		Vec3 find_random_tile_pos();

		Vec2 get_size() const;

	public:
		const float TILE_SIZE = 10.0f;

	private:
		Vec2 _size;

		Entity* _ground { nullptr };
		std::vector<Pawn*> _pawns;
	};
}