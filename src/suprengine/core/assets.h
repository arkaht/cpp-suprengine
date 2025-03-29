#pragma once

#include <suprengine/core/render-batch.h>

#include <suprengine/rendering/texture.h>
#include <suprengine/rendering/font.h>
#include <suprengine/rendering/shader.h>

#include <suprengine/utils/curve.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>

#include <filewatch/FileWatch.hpp>

#include <map>

namespace suprengine
{
	class VertexArray;

	class Assets
	{
	public:
		Assets() = delete;

		static void set_render_batch( RenderBatch* render_batch ) { _render_batch = render_batch; }

		static void set_path( rconst_str path ) { _resources_path = path; }
		static std::string get_path() { return _resources_path; }

		static SharedPtr<Texture> load_texture( rconst_str name, rconst_str path, const TextureParams& params = {} );
		static SharedPtr<Texture> get_texture( rconst_str name );

		static SharedPtr<Font> load_font( rconst_str name, rconst_str path, int size = 12 );
		static SharedPtr<Font> get_font( rconst_str path, int size );

		static SharedPtr<Shader> load_shader( rconst_str name, rconst_str vtx_path, rconst_str frg_path, rconst_str tsc_path = "", rconst_str tse_path = "", rconst_str geo_path = "", bool append_resources_path = true );
		static SharedPtr<Shader> get_shader( rconst_str name );

		static SharedPtr<Model> add_model( rconst_str name, SharedPtr<Model> model );
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
				std::is_same_v<IdentifierType, std::string> || std::is_same_v<IdentifierType, const char*>,
				std::vector<IdentifierType>
			>
			get_assets_as_ids()
		{
			// Create a constexpr lambda to compile the function 
			// with the map of the given asset type.
			auto get_related_assets_map = [&]() constexpr -> std::map<std::string, SharedPtr<AssetType>>&
			{
				if constexpr ( std::is_same_v<AssetType, Texture> )
				{
					return _textures;
				}
				else if constexpr ( std::is_same_v<AssetType, Font> )
				{
					return _fonts;
				}
				else if constexpr ( std::is_same_v<AssetType, Shader> )
				{
					return _shaders;
				}
				else if constexpr ( std::is_same_v<AssetType, Model> )
				{
					return _models;
				}
				else if constexpr ( std::is_same_v<AssetType, Curve> )
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

		static std::map<std::string, SharedPtr<Texture>> _textures;
		static std::map<std::string, SharedPtr<Font>> _fonts;
		static std::map<std::string, SharedPtr<Shader>> _shaders;
		static std::map<std::string, SharedPtr<Model>> _models;
		static std::map<std::string, SharedPtr<Curve>> _curves;

		static std::vector<SharedPtr<filewatcher>> _filewatchers;

		static RenderBatch* _render_batch;
		static std::string _resources_path;
		static Assimp::Importer _importer;
		static curve_x::CurveSerializer _curve_serializer;

		static bool _read_file( rconst_str path, std::string* data );

		static VertexArray* load_mesh( const aiMesh* mesh );
		static std::vector<Mesh*> load_node( const aiNode* node, const aiScene* scene );
	};
}
