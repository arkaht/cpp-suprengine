#pragma once

#include "texture.h"
#include "font.h"

#include <suprengine/opengl/shader.h>

#include <map>

namespace suprengine
{
	class Assets
	{
	public:
		static const std::string PRIMITIVE_CUBE_PATH;
		static const std::string PRIMITIVE_SPHERE_PATH;

		Assets() = delete;

		static void set_render_batch( RenderBatch* render_batch ) { _render_batch = render_batch; }
		
		static void set_path( rconst_str path ) { _resources_path = path; }
		static std::string get_path() { return _resources_path; }

		static Texture* get_texture( rconst_str path, const TextureParams& params = {}, bool append_resources_path = true );
		static Font* get_font( rconst_str path, int size = 12, bool append_resources_path = true );

		static Shader* load_shader( rconst_str name, rconst_str vtx_path, rconst_str frg_path, rconst_str tsc_path = "", rconst_str tse_path = "", rconst_str geo_path = "", bool append_resources_path = true );
		static Shader* get_shader( rconst_str name );

		static Mesh* get_mesh( rconst_str path, bool append_resources_path = true );

		static void release();

	private:
		static std::map<std::string, Texture*> _textures;
		static std::map<std::string, Font*> _fonts;
		static std::map<std::string, Shader*> _shaders;
		static std::map<std::string, Mesh*> _meshes;

		static RenderBatch* _render_batch;
		static std::string _resources_path;

		static Shader* load_shader_from_file( rconst_str vtx_path, rconst_str frg_path, rconst_str tsc_path, rconst_str tse_path, rconst_str geo_path );
		static Mesh* load_mesh_from_file( rconst_str path, bool append_resources_path );
	};
}
