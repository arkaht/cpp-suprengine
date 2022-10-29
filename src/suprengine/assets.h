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
		static std::string path;
	public:
		Assets() = delete;

		static void set_render_batch( RenderBatch* _render_batch ) { render_batch = _render_batch; }
		static void set_path( std::string _path ) { path = _path; }

		static Texture* get_texture( const std::string& filename );
		static void release();
	};
}
