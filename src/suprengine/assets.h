#pragma once

#include "texture.h"
#include <map>

namespace suprengine
{
	class Assets
	{
	private:
		static std::map<std::string, Texture*> textures;

		static RenderBatch* render_batch;
	public:
		Assets() = delete;

		static void set_render_batch( RenderBatch* _render_batch ) { render_batch = _render_batch; }

		static Texture* get_texture( const std::string& filename );
		static void release();
	};
}
