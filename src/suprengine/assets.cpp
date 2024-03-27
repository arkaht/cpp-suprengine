#include "assets.h"

#include <iostream>
#include <sstream>
#include <fstream>
#include <filesystem>

#include <rapidjson/document.h>

#include <assimp/postprocess.h>

#include <suprengine/model.h>
#include <suprengine/vertex-array.h>

using namespace suprengine;

std::map<std::string, Texture*> Assets::_textures;
std::map<std::string, Font*> Assets::_fonts;
std::map<std::string, Shader*> Assets::_shaders;
std::map<std::string, Model*> Assets::_models;
std::map<std::string, ref<Curve>> Assets::_curves;

RenderBatch* Assets::_render_batch { nullptr };
std::string Assets::_resources_path { "" };
Assimp::Importer Assets::_importer;
curve_x::CurveSerializer Assets::_curve_serializer;

const std::string Assets::CUBE_PATH { "assets/suprengine/meshes/cube.fbx" };
const std::string Assets::SPHERE_PATH { "assets/suprengine/meshes/sphere.fbx" };

Texture* Assets::load_texture( rconst_str name, rconst_str path, const TextureParams& params )
{
	_textures[name] = _render_batch->load_texture( path, params );
	return _textures[name];
}

Texture* Assets::get_texture( rconst_str name )
{
	//  check texture
	auto itr = _textures.find( name );
	if ( itr == _textures.end() )
	{
		Logger::error( "Failed to get texture '" + name + "', either not loaded or the name is wrong!" );
		return nullptr;
	}

	//  get from textures
	return (*itr).second;
}

Font* Assets::load_font( rconst_str name, rconst_str path, int size )
{
	std::string key = name + std::to_string( size );

	_fonts[key] = Font::load( path, size );
	return _fonts[key];
}

Font* Assets::get_font( rconst_str name, int size )
{
	std::string key = name + std::to_string( size );

	//  check font
	auto itr = _fonts.find( key );
	if ( itr == _fonts.end() )
	{
		Logger::error( "Failed to get font '" + key + "', either not loaded or the name is wrong!" );
		return nullptr;
	}

	//  get from textures
	return (*itr).second;
}

Shader* Assets::load_shader( rconst_str name, rconst_str vtx_filename, rconst_str frg_filename, rconst_str tsc_filename, rconst_str tse_filename, rconst_str geo_filename, bool append_resources_path )
{
	_shaders[name] = load_shader_from_file( vtx_filename, frg_filename, tsc_filename, tse_filename, geo_filename );
	return get_shader( name );
}

Shader* Assets::get_shader( rconst_str name ) 
{ 
	auto itr = _shaders.find( name );
	if ( itr == _shaders.end() )
	{
		Logger::error( "Failed to get shader '" + name + "', either not loaded or the name is wrong!" );
		return nullptr;
	}

	//  get from textures
	return (*itr).second;
}

Model* Assets::load_model( rconst_str name, rconst_str path, rconst_str shader_name )
{
	//  set import flags
	int flags = aiProcess_Triangulate 
			  //| aiProcess_MakeLeftHanded
			  | aiProcess_FlipUVs 
			  | aiProcess_JoinIdenticalVertices;
	
	//  load mesh
	auto scene = _importer.ReadFile( path, flags );
	if ( scene == nullptr ) 
	{
		Logger::error( "Failed to load mesh '" + path + "', file path is probably wrong!" );
		return nullptr;
	}

	//  load model
	auto meshes = load_node( scene->mRootNode, scene );
	if ( meshes.size() == 0u )
	{
		Logger::error( "Failed to load mesh '" + path + "', no mesh couldn't be loaded!" );
		return nullptr;
	}

	//  create model
	auto model = new Model( std::move( meshes ), shader_name );
	
	//  register and return
	_models[name] = model;
	return model;
}

Model* Assets::get_model( rconst_str name )
{
	auto itr = _models.find( name );
	if ( itr == _models.end() )
	{
		Logger::error( "Failed to get model '" + name + "', either not loaded or the name is wrong!" );
		return nullptr;
	}

	return (*itr).second;
}

void Assets::load_curves_in_folder( 
	rconst_str path, 
	bool is_recursive,
	rconst_str name_prefix
)
{
	Logger::info( "Loading curves in folder '" + path + "':" );

	std::filesystem::directory_iterator itr( path );
	for ( const auto& entry : itr )
	{
		auto& path = entry.path();

		//  ignore directories
		if ( entry.is_directory() )
		{
			//  if is recursive loading, load the folder
			if ( is_recursive )
			{
				load_curves_in_folder( 
					path.string(), 
					true, 
					path.filename().string() + "/"
				);
			}
			continue;
		}

		//  load curve file
		load_curve( 
			name_prefix + path.filename().replace_extension().string(),
			path.string()
		);
	}
}

ref<Curve> Assets::load_curve( 
	rconst_str name, 
	rconst_str path 
)
{
	//  read curve file
	std::string data;
	if ( !_read_file( path, &data ) ) return nullptr;

	//  unserialize curve and store it
	Curve temporary = _curve_serializer.unserialize( data );
	_curves[name] = std::make_shared<Curve>( temporary );

	Logger::info( "Registered Curve asset as '" + name + "'." );
	return _curves[name];
}

ref<Curve> Assets::get_curve( rconst_str name )
{
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
	//  release textures
	for ( auto& pair : _textures )
		delete pair.second;
	_textures.clear();

	//  release fonts
	for ( auto& pair : _fonts )
		delete pair.second;
	_fonts.clear();

	//  release shaders
	for ( auto& pair : _shaders )
		delete pair.second;
	_shaders.clear();

	//  release meshes
	for ( auto& pair : _models )
		delete pair.second;
	_models.clear();

	//  release curves
	_curves.clear();
}

bool Assets::_read_file( rconst_str path, std::string* data )
{
	const char* c_path = path.c_str();
	std::ifstream file;
	file.open( c_path );

	//  prepare output
	*data = "";

	//  check file exists
	if ( !file.is_open() )
	{
		Logger::error( "File '" + path + "' doesn't exists, aborting reading from file!" );
		return false;
	}

	//  read file's content
	for ( std::string line; std::getline( file, line ); )
	{
		*data += line + '\n';
	}
	file.close();

	Logger::info( "Read file '" + path + "'." );
	return true;
}

Shader* Assets::load_shader_from_file(
	rconst_str vtx_filename, 
	rconst_str frg_filename, 
	rconst_str tsc_filename, 
	rconst_str tse_filename, 
	rconst_str geo_filename 
)
{
	// 1. Retrieve the vertex/fragment source code from filePath
	std::string vertexCode;
	std::string fragmentCode;
	std::string tcCode;
	std::string teCode;
	std::string geometryCode;
	try
	{
		// Open files
		std::ifstream vertexShaderFile( vtx_filename );
		if ( !vertexShaderFile.is_open() )
		{
			throw std::exception( "vertex file not found!" );
		}

		std::ifstream fragmentShaderFile( frg_filename );
		if ( !fragmentShaderFile.is_open() )
		{
			throw std::exception( "fragment file not found!" );
		}
		
		std::stringstream vShaderStream, fShaderStream;
		// Read file's buffer contents into streams
		vShaderStream << vertexShaderFile.rdbuf();
		fShaderStream << fragmentShaderFile.rdbuf();
		// close file handlers
		vertexShaderFile.close();
		fragmentShaderFile.close();
		// Convert stream into string
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
		// If tess control shader path is present, also load a tess control shader
		if ( tsc_filename != "" )
		{
			std::ifstream tessControlShaderFile( tsc_filename );
			std::stringstream tcShaderStream;
			tcShaderStream << tessControlShaderFile.rdbuf();
			tessControlShaderFile.close();
			tcCode = tcShaderStream.str();
		}
		// If tess evaluation shader path is present, also load a tess evaluation shader
		if ( tse_filename != "" )
		{
			std::ifstream tessEvalShaderFile( tse_filename );
			std::stringstream teShaderStream;
			teShaderStream << tessEvalShaderFile.rdbuf();
			tessEvalShaderFile.close();
			teCode = teShaderStream.str();
		}
		// If geometry shader path is present, also load a geometry shader
		if ( geo_filename != "" )
		{
			std::ifstream geometryShaderFile( geo_filename );
			std::stringstream gShaderStream;
			gShaderStream << geometryShaderFile.rdbuf();
			geometryShaderFile.close();
			geometryCode = gShaderStream.str();
		}
	}
	catch ( std::exception e )
	{
		Logger::error( "SHADER: failed to read shader files (vertex: '" + vtx_filename + "'; fragment: '" + frg_filename + "'): '" + e.what() + "'\n" );
		return nullptr;
	}
	const GLchar* vShaderCode = vertexCode.c_str();
	const GLchar* fShaderCode = fragmentCode.c_str();
	const GLchar* tcShaderCode = tcCode.c_str();
	const GLchar* teShaderCode = teCode.c_str();
	const GLchar* gShaderCode = geometryCode.c_str();

	// 2. Now create shader object from source code
	Logger::info( "compiling shaders (vertex: '" + vtx_filename + "'; fragment: '" + frg_filename + "')" );
	Shader* shader = new Shader();
	shader->compile( vShaderCode, fShaderCode,
		tsc_filename != "" ? tcShaderCode : nullptr,
		tse_filename != "" ? teShaderCode : nullptr,
		geo_filename != "" ? gShaderCode : nullptr );

	return shader;
}

VertexArray* Assets::load_mesh( const aiMesh* mesh )
{
	printf( "| Mesh Name: '%s'\n", mesh->mName.C_Str() );

	const VertexArrayPreset& preset = 
		VertexArrayPreset::Position3_Normal3_UV2;
	
	//  get vertices count
	unsigned int vertices_count = mesh->mNumVertices;
	printf( "> Vertices Count: %d\n", vertices_count );

	//  init vertices and indices containers
	std::vector<float> vertices( vertices_count * preset.stride );
	std::vector<unsigned int> indices;

	bool has_normals = mesh->HasNormals();
	bool has_uvs = mesh->HasTextureCoords( 0 );

	printf( "> Normals: %s\n", has_normals ? "true" : "false" );
	printf( "> UVs: %s\n", has_uvs ? "true" : "false" );

	//  copy vertices
	for ( size_t i = 0; i < vertices_count; i++ )
	{
		size_t vertex_id = i * preset.stride;

		//  position
		//  NOTE: Y and Z axes are swapped, models won't show properly otherwise
		auto vertex = mesh->mVertices[i];
		vertices[vertex_id + 0] = vertex.x;
		vertices[vertex_id + 1] = vertex.y;
		vertices[vertex_id + 2] = vertex.z;
		
		//  normal
		if ( has_normals ) 
		{
			//  NOTE: similarly to vertex, Y and Z axes are swapped
			auto normal = mesh->mNormals[i];
			vertices[vertex_id + 3] = normal.x;
			vertices[vertex_id + 4] = normal.y;
			vertices[vertex_id + 5] = normal.z;
		}

		//  uv
		if ( has_uvs )
		{
			auto uv = mesh->mTextureCoords[0][i];
			vertices[vertex_id + 6] = uv.x;
			vertices[vertex_id + 7] = uv.y;
		}
	}

	//  copy indices
	for ( size_t i = 0; i < mesh->mNumFaces; i++ )
	{
		auto& face = mesh->mFaces[i];
		for ( size_t j = 0; j < face.mNumIndices; j++ )
		{
			indices.push_back( face.mIndices[j] );
		}
	}
	printf( "> Indices Count: %d\n", (int)indices.size() );

	//  create vertex array
	auto vertex_array = new VertexArray(
		preset,
		vertices.data(),
		vertices_count,
		indices.data(),
		(unsigned int)indices.size()
	);
	return vertex_array;
}

std::vector<Mesh*> Assets::load_node( const aiNode* node, const aiScene* scene )
{
	std::vector<Mesh*> meshes;

	printf( "| Root Name: '%s'\n", node->mName.C_Str() );

	//  load meshes
	printf( "> Meshes Count: %d\n", node->mNumMeshes );
	for ( size_t i = 0; i < node->mNumMeshes; i++ )
	{
		auto vertex_array = load_mesh( scene->mMeshes[node->mMeshes[i]] );
		meshes.push_back( new Mesh( vertex_array ) );
	}

	//  recursive children loading
	printf( "> Children Count: %d\n", node->mNumChildren );
	for ( size_t i = 0; i < node->mNumChildren; i++ )
	{
		auto new_meshes = load_node( node->mChildren[i], scene );
		meshes.insert( meshes.end(), new_meshes.begin(), new_meshes.end() );
	}

	return meshes;
}