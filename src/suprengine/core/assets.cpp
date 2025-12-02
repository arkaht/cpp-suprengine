#include "assets.h"

#include <filesystem>

#include <assimp/postprocess.h>

#include <suprengine/data/shader/shader-asset-info.h>

#include <suprengine/rendering/model.h>
#include <suprengine/rendering/vertex-array.h>
#include <suprengine/rendering/shader-program.h>
#include <suprengine/rendering/shader.h>

#include <suprengine/utils/logger.h>

using namespace suprengine;

std::map<std::string, SharedPtr<Texture>> Assets::_textures;
std::map<std::string, SharedPtr<Font>> Assets::_fonts;
std::map<std::string, SharedPtr<ShaderProgram>> Assets::_shader_programs;
std::map<std::string, SharedPtr<Model>> Assets::_models;
std::map<std::string, SharedPtr<Curve>> Assets::_curves;

std::vector<SharedPtr<Assets::filewatcher>> Assets::_filewatchers;

RenderBatch* Assets::_render_batch { nullptr };
std::string Assets::_resources_path { "" };
Assimp::Importer Assets::_importer;
curve_x::CurveSerializer Assets::_curve_serializer;

namespace
{
	bool read_file( const std::string& path, std::string& out_data )
	{
		std::ifstream file;
		file.open( path.c_str() );

		//  Check whether file exists
		if ( !file.is_open() )
		{
			Logger::error( "File '" + path + "' doesn't exists, aborting reading from file!" );
			return false;
		}

		std::stringstream stream {};
		stream << file.rdbuf();
		out_data = stream.str();

		file.close();
		return true;
	}
}

SharedPtr<Texture> Assets::load_texture( rconst_str name, rconst_str path, const TextureParams& params )
{
	Logger::info(
		"Loading texture '%s' at path '%s' (F: %d)",
		*name, *path,
		params.filtering
	);

	_textures[name] = _render_batch->load_texture( path, params );
	return _textures[name];
}

SharedPtr<Texture> Assets::get_texture( rconst_str name )
{
	//  check texture
	const auto itr = _textures.find( name );
	if ( itr == _textures.end() )
	{
		Logger::error( "Failed to get texture '" + name + "', either not loaded or the name is wrong!" );
		return nullptr;
	}

	//  get from textures
	return (*itr).second;
}

SharedPtr<Font> Assets::load_font( rconst_str name, rconst_str path, int size )
{
	const std::string key = name + std::to_string( size );

	_fonts[key] = Font::load( path, size );
	return _fonts[key];
}

SharedPtr<Font> Assets::get_font( rconst_str name, int size )
{
	const std::string key = name + std::to_string( size );

	//  check font
	const auto itr = _fonts.find( key );
	if ( itr == _fonts.end() )
	{
		Logger::error( "Failed to get font '" + key + "', either not loaded or the name is wrong!" );
		return nullptr;
	}

	//  get from textures
	return (*itr).second;
}

SharedPtr<ShaderProgram> Assets::load_shader_program( const ShaderProgramAssetInfo& asset_info )
{
	ASSERT( !asset_info.name.empty() );
	ASSERT( !asset_info.shaders.empty() );

	Logger::info(
		"Loading shader program '%s' with %d shaders",
		*asset_info.name, asset_info.shaders.size()
	);

	std::vector<Shader> shaders {};
	shaders.reserve( asset_info.shaders.size() );

	// Gather and compile all shaders
	std::string code {};
	for ( const ShaderAssetInfo& shader_info : asset_info.shaders )
	{
		ASSERT( !shader_info.code_path.empty() )

		if ( !read_file( shader_info.code_path, code ) )
		{
			ASSERT( false );
			return nullptr;
		}

		// Constructor handle code compiling
		const Shader& shader = shaders.emplace_back( shader_info.code_path, code, shader_info.type );
		if ( !shader.is_valid() )
		{
			ASSERT( shader.is_valid() );
			return nullptr;
		}

		Logger::info(
			"Compiled successfully %s shader '%s' (ID: %d)",
			get_shader_type_name( shader_info.type ), *shader.get_name(), shader.get_id()
		);
	}

	ASSERT_MSG( shaders.size() == asset_info.shaders.size(), "Failed to load all required shaders" );

	// Create, link and validate the shader program
	SharedPtr<ShaderProgram> shader_program( new ShaderProgram( asset_info.name, shaders ) );
	if ( !shader_program->is_valid() )
	{
		ASSERT( shader_program->is_valid() );
		return nullptr;
	}

	Logger::info(
		"Linked and validated successfully shader program '%s' (ID: %d)",
		*shader_program->get_name(), shader_program->get_id()
	);
	shader_program->print_all_params();

	_shader_programs[asset_info.name] = shader_program;
	return shader_program;
}

SharedPtr<ShaderProgram> Assets::get_shader_program( rconst_str name )
{ 
	const auto itr = _shader_programs.find( name );
	if ( itr == _shader_programs.end() )
	{
		Logger::error( "Failed to get shader program '" + name + "', either not loaded or the name is wrong!" );
		return nullptr;
	}

	// Get from shaders
	return (*itr).second;
}

SharedPtr<Model> Assets::add_model( rconst_str name, SharedPtr<Model> model )
{
	_models[name] = model;
	return model;
}

SharedPtr<Model> Assets::load_model( rconst_str name, rconst_str path, rconst_str shader_name )
{
	//	Set import flags
	const int flags = aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices;
	
	//	Load mesh
	const aiScene* scene = _importer.ReadFile( path, flags );
	if ( scene == nullptr ) 
	{
		Logger::error( "Failed to load model at path '%s', file not found or corrupted!", *path );
		return nullptr;
	}

	Logger::info( "Loading model '%s' at path '%s'", *name, *path );

	//	Load all meshes from the root node
	std::vector<Mesh*> meshes = load_node( scene->mRootNode, scene );
	if ( meshes.empty() )
	{
		Logger::error( "Failed to load model at path '%s', the model doesn't contain any meshes!", *path );
		return nullptr;
	}

	//	Create model
	SharedPtr<Model> model = std::make_shared<Model>( std::move( meshes ), shader_name );
	
	//	Register and return
	_models[name] = model;
	return model;
}

SharedPtr<Model> Assets::get_model( rconst_str name )
{
	const auto itr = _models.find( name );
	if ( itr == _models.end() )
	{
		Logger::error( "Failed to get model '%s', either not loaded or the name is wrong!", *name );
		return nullptr;
	}

	return (*itr).second;
}

void Assets::load_curves_in_folder( 
	rconst_str path, 
	bool is_recursive,
	bool should_auto_reload,
	rconst_str name_prefix
)
{
	Logger::info( "Loading curves in folder '%s':", *path );

	std::filesystem::directory_iterator itr( path );
	for ( const auto& entry : itr )
	{
		auto& file_path = entry.path();

		//  ignore directories
		if ( entry.is_directory() )
		{
			//  if is recursive loading, load the folder
			if ( is_recursive )
			{
				load_curves_in_folder( 
					file_path.string(), 
					true, 
					false,
					name_prefix + file_path.filename().string() + "/"
				);
			}
			continue;
		}

		//  load curve file
		load_curve( 
			name_prefix + file_path.filename().replace_extension().string(),
			file_path.string()
		);
	}

	if ( should_auto_reload )
	{
		//  setup file watcher and auto reload
		auto watcher = std::make_shared<filewatcher>(
			path,
			[name_prefix, path]( const std::string& file, const filewatch::Event event )
			{
				//  concatenate the file path with the directory
				const std::filesystem::path file_path = path + file;

				//  get asset name prefix from file path
				const std::string file_name = file_path.filename().string();
				std::string prefix = name_prefix + file;
				std::replace( prefix.begin(), prefix.end(), '\\', '/' );
				if ( auto index = prefix.find( file_name ) )
				{
					prefix.erase( index );
				}

				/*const auto last_write_time = std::filesystem::last_write_time( file_path );
				std::cout << "File event on '" 
						  << file_path << "', prefix: '"
						  << prefix << "', last write time: " 
						  << last_write_time << std::endl;*/

				switch( event )
				{
					//  TODO: Find a workaround because of the modified 
					//		  event always being called twice.
					case filewatch::Event::modified:

						Logger::info( "Auto-reloading Curve file '" + file_path.string() + "'" );

						load_curve( 
							prefix + file_path.filename().replace_extension().string(),
							file_path.string()
						);
						break;
				}
			}
		);
		_filewatchers.push_back( watcher );
		Logger::info( "Added a file watcher on directory '" + path + "'" );
	}
}

SharedPtr<Curve> Assets::load_curve( 
	rconst_str name, 
	rconst_str path 
)
{
	//  Read curve file
	std::string data;
	if ( !read_file( path, data ) ) return nullptr;

	//  Un-serialize curve and store it
	Curve temporary = _curve_serializer.unserialize( data );
	_curves[name] = std::make_shared<Curve>( temporary );

	Logger::info( "Registered Curve asset as '" + name + "'." );
	return _curves[name];
}

SharedPtr<Curve> Assets::get_curve( rconst_str name )
{
	if ( name.empty() ) return nullptr;

	auto itr = _curves.find( name );
	if ( itr == _curves.end() )
	{
		Logger::error( "Failed to get curve '" + name + "', either not loaded or the name is wrong!" );
		return nullptr;
	}

	return (*itr).second;
}

void Assets::release()
{
	//  Release textures
	_textures.clear();

	//  Release fonts
	_fonts.clear();

	//  Release shaders
	_shader_programs.clear();

	//  Release meshes
	_models.clear();

	//  Release curves
	_curves.clear();
}

VertexArray* Assets::load_mesh( const aiMesh* mesh )
{
	const VertexArrayPreset& preset = VertexArrayPreset::Position3_Normal3_UV2;
	
	//	Create vertices and indices containers
	const size_t vertices_count = mesh->mNumVertices;
	std::vector<float> vertices( vertices_count * preset.stride );
	std::vector<unsigned int> indices;

	const bool has_normals = mesh->HasNormals();
	const bool has_uvs = mesh->HasTextureCoords( 0 );

	//	Copy vertices
	for ( size_t i = 0; i < vertices_count; i++ )
	{
		size_t vertex_id = i * preset.stride;

		//	Position
		const aiVector3D& vertex = mesh->mVertices[i];
		vertices[vertex_id + 0] = vertex.x;
		vertices[vertex_id + 1] = vertex.y;
		vertices[vertex_id + 2] = vertex.z;
		
		//	Normal
		if ( has_normals ) 
		{
			const aiVector3D& normal = mesh->mNormals[i];
			vertices[vertex_id + 3] = normal.x;
			vertices[vertex_id + 4] = normal.y;
			vertices[vertex_id + 5] = normal.z;
		}

		//	UV
		if ( has_uvs )
		{
			const aiVector3D& uv = mesh->mTextureCoords[0][i];
			vertices[vertex_id + 6] = uv.x;
			vertices[vertex_id + 7] = uv.y;
		}
	}

	//	Copy indices
	for ( size_t i = 0; i < mesh->mNumFaces; i++ )
	{
		auto& face = mesh->mFaces[i];
		for ( size_t j = 0; j < face.mNumIndices; j++ )
		{
			indices.push_back( face.mIndices[j] );
		}
	}

	const size_t indices_count = indices.size();

	Logger::info(
		"Loaded mesh '%s' (V: %d; I: %d; N: %s; UV: %s)",
		mesh->mName.C_Str(),
		vertices_count, indices_count,
		has_normals ? "true" : "false", has_uvs ? "true" : "false"
	);

	//  Create vertex array
	auto vertex_array = new VertexArray(
		preset,
		vertices.data(), static_cast<uint32>( vertices_count ),
		indices.data(), static_cast<uint32>( indices_count )
	);
	return vertex_array;
}

std::vector<Mesh*> Assets::load_node( const aiNode* node, const aiScene* scene )
{
	std::vector<Mesh*> meshes {};
	meshes.reserve( node->mNumMeshes );

	//	Load meshes
	for ( size_t i = 0; i < node->mNumMeshes; i++ )
	{
		auto vertex_array = load_mesh( scene->mMeshes[node->mMeshes[i]] );
		meshes.push_back( new Mesh( vertex_array ) );
	}

	//	Recursively load children nodes
	for ( size_t i = 0; i < node->mNumChildren; i++ )
	{
		auto new_meshes = load_node( node->mChildren[i], scene );
		if ( new_meshes.empty() ) continue;

		meshes.insert( meshes.end(), new_meshes.begin(), new_meshes.end() );
	}

	return meshes;
}