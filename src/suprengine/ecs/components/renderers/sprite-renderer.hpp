#pragma once
#include "renderer.h"

namespace suprengine
{
	class SpriteRenderer : public Renderer
	{
	public:
		Texture* texture { nullptr };
		Rect source { 0.0f, 0.0f, 0.0f, 0.0f };
		Rect dest { 0.0f, 0.0f, 0.0f, 0.0f };
		Vec2 origin { 0.5f, 0.5f };

		SpriteRenderer( Entity* owner, Texture* texture, int priority_order = 0 )
			: texture( texture ), dest( dest ), Renderer( owner, priority_order )
		{
			if ( texture == nullptr ) return;

			size_to_texture();
		}

		void size_to_texture()
		{
			//  auto-size source & dest rectangles
			Vec2 size = texture->get_size();
			source.w = size.x, source.h = size.y;
			dest.w = size.x, dest.h = size.y;
		}
	
		void render() override
		{
			if ( texture == nullptr ) return;

			Transform* transf = owner->transform;
			render_batch->draw_texture( 
				source,
				owner->transform->get_rect( dest ),
				owner->transform->rotation.get_z_angle(),
				origin,
				texture,
				modulate
			);
		}

		RenderPhase get_render_phase() const override { return RenderPhase::SPRITE; }
	};
}