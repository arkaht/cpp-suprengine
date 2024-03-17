#pragma once

#include "texture.h"
#include "font.h"

#include <suprengine/shader.h>
#include <suprengine/model.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>

#include <curve-x/curve-serializer.h>

#include <map>

namespace suprengine
{
	using curve_x::Curve;

	class VertexArray;

	//  TODO: handle assets with ref<>
	class Assets
	{
	public:
		static const std::string CUBE_PATH;
		static const std::string SPHERE_PATH;

		Assets() = delete;

		static void set_render_batch( RenderBatch* render_batch ) { _render_batch = render_batch; }
		
		static void set_path( rconst_str path ) { _resources_path = path; }
		static std::string get_path() { return _resources_path; }

		static Texture* load_texture( rconst_str name, rconst_str path, const TextureParams& params = {} );
		static Texture* get_texture( rconst_str name );
		
		static Font* load_font( rconst_str name, rconst_str path, int size = 12 );
		static Font* get_font( rconst_str path, int size );

		static Shader* load_shader( rconst_str name, rconst_str vtx_path, rconst_str frg_path, rconst_str tsc_path = "", rconst_str tse_path = "", rconst_str geo_path = "", bool append_resources_path = true );
		static Shader* get_shader( rconst_str name );

		static Model* load_model( rconst_str name, rconst_str path, rconst_str shader_name = "" );
		static Model* get_model( rconst_str name );

		static ref<Curve> load_curve( 
			rconst_str name, 
			rconst_str path 
		);
		static ref<Curve> get_curve( rconst_str name );

		static void release();

	private:
		static std::map<std::string, Texture*> _textures;
		static std::map<std::string, Font*> _fonts;
		static std::map<std::string, Shader*> _shaders;
		static std::map<std::string, Model*> _models;
		static std::map<std::string, ref<Curve>> _curves;

		static RenderBatch* _render_batch;
		static std::string _resources_path;
		static Assimp::Importer _importer;
		static curve_x::CurveSerializer _curve_serializer;

		static bool _read_file( rconst_str path, std::string* data );

		static Shader* load_shader_from_file( rconst_str vtx_path, rconst_str frg_path, rconst_str tsc_path, rconst_str tse_path, rconst_str geo_path );
		
		static VertexArray* load_mesh( const aiMesh* mesh );
		static std::vector<Mesh*> load_node( const aiNode* node, const aiScene* scene );
	};
}
