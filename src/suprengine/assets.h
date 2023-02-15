#pragma once

#include "texture.h"
#include "font.h"

#include <suprengine/opengl/shader.h>

#include <map>

namespace suprengine
{
	class Assets
	{
	private:
		static std::map<std::string, Texture*> textures;
		static std::map<std::string, Font*> fonts;
		static std::map<std::string, Shader*> shaders;

		static RenderBatch* render_batch;
		static std::string path;
	public:
		Assets() = delete;

		static void set_render_batch( RenderBatch* _render_batch ) { render_batch = _render_batch; }
		
		static void set_path( rconst_str _path ) { path = _path; }
		static std::string get_path() { return path; }

		static Texture* get_texture( rconst_str filename );
		static Font* get_font( rconst_str filename, int size = 12 );

		static Shader* load_shader( rconst_str name, rconst_str vtx_filename, rconst_str frg_filename, rconst_str tsc_filename = "", rconst_str tse_filename = "", rconst_str geo_filename = "" );
		static Shader* get_shader( rconst_str name );

		static void release();
	private:
		static Shader* load_shader_from_file( rconst_str vtx_filename, rconst_str frg_filename, rconst_str tsc_filename, rconst_str tse_filename, rconst_str geo_filename );
	};
}
