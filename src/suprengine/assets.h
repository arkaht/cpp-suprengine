#pragma once

#include "texture.h"
#include "font.h"

#include <suprengine/shader.h>
#include <suprengine/model.h>
#include <suprengine/curve.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>

#include <filewatch/FileWatch.hpp>

#include <map>

namespace suprengine
{
	class VertexArray;

	//  TODO: handle assets with SharedPtr<>
	class Assets
	{
	public:
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

		static SharedPtr<Model> load_model( rconst_str name, rconst_str path, rconst_str shader_name = "" );
		static SharedPtr<Model> get_model( rconst_str name );

		static void load_curves_in_folder(
			rconst_str path,
			bool is_recursive = false,
			bool should_auto_reload = false,
			rconst_str name_prefix = ""
		);
		static SharedPtr<Curve> load_curve(
			rconst_str name,
			rconst_str path
		);
		static SharedPtr<Curve> get_curve( rconst_str name );

		/*
		 * Get a list of all assets identifiers of a given type.
		 * Only works with 'std::string' and 'const char*' for IdentifierType.
		 */
		template <typename AssetType, typename IdentifierType>
		static 
			std::enable_if_t<
				std::is_same<std::string, IdentifierType>::value || std::is_same<const char*, IdentifierType>::value, 
				std::vector<IdentifierType>
			>
			get_assets_as_ids()
		{
			// Create a constexpr lambda to compile the function 
			// with the map of the given asset type.
			auto get_related_assets_map = [&]() constexpr -> std::map<std::string, SharedPtr<AssetType>>&
			{
				if constexpr ( std::is_same<Texture, AssetType>::value )
				{
					return _textures;
				}
				else if constexpr ( std::is_same<Font, AssetType>::value )
				{
					return _fonts;
				}
				else if constexpr ( std::is_same<Shader, AssetType>::value )
				{
					return _shaders;
				}
				else if constexpr ( std::is_same<Model, AssetType>::value )
				{
					return _models;
				}
				else if constexpr ( std::is_same<Curve, AssetType>::value )
				{
					return _curves;
				}
			};

			// Get the map of the asset type
			auto& assets_map = get_related_assets_map();
			
			// Reserve enough elements to only do one allocation
			std::vector<IdentifierType> assets_as_ids {};
			assets_as_ids.reserve( assets_map.size() );

			for ( auto& pair : assets_map )
			{
				assets_as_ids.emplace_back( pair.first.c_str() );
			}

			return assets_as_ids;
		}

		static void release();

	private:
		using filewatcher = filewatch::FileWatch<std::string>;

		static std::map<std::string, Texture*> _textures;
		static std::map<std::string, Font*> _fonts;
		static std::map<std::string, Shader*> _shaders;
		static std::map<std::string, SharedPtr<Model>> _models;
		static std::map<std::string, SharedPtr<Curve>> _curves;

		static std::vector<SharedPtr<filewatcher>> _filewatchers;

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
