#pragma once

#include "texture.h"
#include "font.h"
#include <map>

namespace suprengine
{
	class Assets
	{
	private:
		static std::map<std::string, Texture*> textures;
		static std::map<std::string, Font*> fonts;

		static RenderBatch* render_batch;
		static std::string path;
	public:
		Assets() = delete;

		static void set_render_batch( RenderBatch* _render_batch ) { render_batch = _render_batch; }
		
		static void set_path( const std::string& _path ) { path = _path; }
		static std::string get_path() { return path; }

		static Texture* get_texture( const std::string& filename );
		static Font* get_font( const std::string& filename, int size = 12 );
		static void release();
	};
}
