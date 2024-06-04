#pragma once

#include <suprengine/entity.h>
#include <suprengine/components/renderers/model-renderer.hpp>

namespace eks
{
	using namespace suprengine;

	class Pawn : public Entity
	{
	public:
		Pawn( const ref<Model>& model );

		void update_this( float dt ) override;

		void move_to( const Vec3& target );

		void update_tile_pos();
		Vec3 get_tile_pos() const;

	public:
		const float TILE_SIZE = 10.0f;
		//  In tile per seconds, how fast the pawn is?
		const float MOVE_SPEED = 4.0f;

	private:
		ref<ModelRenderer> _renderer; 

		float _move_progress = 1.0f;
		Vec3 _move_to;
		Vec3 _move_from;

		//  Position in tile coordinates
		Vec3 _tile_pos;
	};
}