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
		Pawn( World* world, const SharedPtr<Model>& model );

		void setup() override;
		void update_this( float dt ) override;

		void move_to( const Vec3& target );

		void set_tile_pos( const Vec3& tile_pos );
		void update_tile_pos();
		Vec3 get_tile_pos() const;

	public:
		//  In tile per seconds, how fast the pawn is?
		const float MOVE_SPEED = 4.0f;

	private:
		World* _world;
		SharedPtr<Model> _model;
		SharedPtr<ModelRenderer> _renderer; 

		float _move_progress = 1.0f;
		Vec3 _move_to;
		Vec3 _move_from;

		//  Position in tile coordinates
		Vec3 _tile_pos;
	};
}