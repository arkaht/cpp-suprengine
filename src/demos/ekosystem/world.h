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

		SharedPtr<Pawn> create_pawn( const Vec3& tile_pos );

		void clear();

		Vec3 find_random_tile_pos();

		Vec2 get_size() const;

	public:
		const float TILE_SIZE = 10.0f;

	private:
		Vec2 _size;

		WeakPtr<Entity> _wk_ground;
		std::vector<WeakPtr<Pawn>> _wk_pawns;
	};
}