#pragma once

#include <suprengine/entity.h>
#include <suprengine/components/renderers/model-renderer.hpp>

#include "world.h"

namespace eks
{
	using namespace suprengine;

	class Pawn : public Entity
	{
	public:
		Pawn( World* world, SafePtr<PawnData> data );

		void setup() override;
		void update_this( float dt ) override;

		void move_to( const Vec3& target );

		void set_tile_pos( const Vec3& tile_pos );
		void update_tile_pos();
		Vec3 get_tile_pos() const;

		std::string get_name() const;

	public:
		SafePtr<PawnData> data;

		float hunger = 0.0f;

	private:
		void _find_food();
		void _find_path_to( const Vec3& target );

	private:
		World* _world;
		SharedPtr<ModelRenderer> _renderer; 

		SafePtr<Pawn> _food_target;

		std::vector<Vec3> _move_path;

		float _move_progress = 1.0f;
		Vec3 _move_to;

		//  Position in tile coordinates
		Vec3 _tile_pos;
	};
}