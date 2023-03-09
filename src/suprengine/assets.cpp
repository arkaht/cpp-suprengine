#include "assets.h"

#include <iostream>
#include <sstream>
#include <fstream>

#include <rapidjson/document.h>

#include <suprengine/opengl/opengl-mesh.hpp>

using namespace suprengine;

std::map<std::string, Texture*> Assets::textures;
std::map<std::string, Font*> Assets::fonts;
std::map<std::string, Shader*> Assets::shaders;
std::map<std::string, Mesh*> Assets::meshes;

RenderBatch* Assets::render_batch { nullptr };
std::string Assets::resources_path { "" };

const std::string Assets::PRIMITIVE_CUBE_PATH { "src/suprengine/assets/meshes/primitives/cube.gpmesh" };
const std::string Assets::PRIMITIVE_SPHERE_PATH { "src/suprengine/assets/meshes/primitives/sphere.gpmesh" };

Texture* Assets::get_texture( rconst_str path, const TextureParams& params, bool append_resources_path )
{
	std::string full_path = append_resources_path ? resources_path + path : path;

	//  TODO: find a way to take params in account even with existing texture
	//  load texture if un-found
	if ( textures.find( path ) == textures.end() )
	{
		textures[path] = render_batch->load_texture( full_path, params );
	}

	//  get from textures
	return textures[path];
}

Font* Assets::get_font( rconst_str path, int size, bool append_resources_path )
{
	std::string full_path = append_resources_path ? resources_path + path : path;
	std::string key = full_path + std::to_string( size );

	//  load texture if un-found
	if ( fonts.find( path ) == fonts.end() )
	{
		fonts[key] = Font::load( full_path, size );
	}

	//  get from textures
	return fonts[key];
}

Shader* Assets::load_shader( rconst_str name, rconst_str vtx_filename, rconst_str frg_filename, rconst_str tsc_filename, rconst_str tse_filename, rconst_str geo_filename, bool append_resources_path )
{
	shaders[name] = load_shader_from_file( vtx_filename, frg_filename, tsc_filename, tse_filename, geo_filename );
	return get_shader( name );
}

Shader* Assets::get_shader( rconst_str name ) { return shaders[name]; }

Mesh* Assets::get_mesh( rconst_str path, bool append_resources_path )
{
	std::string full_path = append_resources_path ? resources_path + path : path;

	if ( meshes.find( full_path ) == meshes.end() )
	{
		meshes[full_path] = load_mesh_from_file( full_path, append_resources_path );
	}

	return meshes[full_path];
}

void Assets::release()
{
	//  release textures
	for ( auto& pair : textures )
		delete pair.second;
	textures.clear();

	//  release fonts
	for ( auto& pair : fonts )
		delete pair.second;
	fonts.clear();

	//  release shaders
	for ( auto& pair : shaders )
		delete pair.second;
	shaders.clear();

	//  release meshes
	for ( auto& pair : meshes )
		delete pair.second;
	meshes.clear();
}

Shader* Assets::load_shader_from_file( rconst_str vtx_filename, rconst_str frg_filename, rconst_str tsc_filename, rconst_str tse_filename, rconst_str geo_filename )
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
		std::ifstream fragmentShaderFile( frg_filename );
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
		std::ostringstream loadError;
		std::string geomShaderFile = "";
		if ( geo_filename != "" )
			geomShaderFile = geo_filename;

		loadError << "ERROR::SHADER: Failed to read shader files " << vtx_filename << " " << frg_filename << " "
			<< geomShaderFile << "\n"
			<< "\n -- --------------------------------------------------- -- "
			<< std::endl;
		Logger::error( loadError.str() );
	}
	const GLchar* vShaderCode = vertexCode.c_str();
	const GLchar* fShaderCode = fragmentCode.c_str();
	const GLchar* tcShaderCode = tcCode.c_str();
	const GLchar* teShaderCode = teCode.c_str();
	const GLchar* gShaderCode = geometryCode.c_str();

	// 2. Now create shader object from source code
	Shader* shader = new Shader();
	shader->compile( vShaderCode, fShaderCode,
		tsc_filename != "" ? tcShaderCode : nullptr,
		tse_filename != "" ? teShaderCode : nullptr,
		geo_filename != "" ? gShaderCode : nullptr );

	return shader;
}

//  TODO: replace this w/ a real 3D model format like .obj/.fbx
Mesh* Assets::load_mesh_from_file( rconst_str path, bool append_resources_path )
{
	std::ifstream file( path );
	if ( !file.is_open() )
	{
		Logger::error( "File not found: Mesh " + path );
	}

	std::stringstream fileStream;
	fileStream << file.rdbuf();
	std::string contents = fileStream.str();
	rapidjson::StringStream jsonStr( contents.c_str() );
	rapidjson::Document doc;
	doc.ParseStream( jsonStr );

	if ( !doc.IsObject() )
	{
		std::ostringstream s;
		s << "Mesh " << path << " is not valid json";
		Logger::error( s.str() );
	}

	OpenGLMesh* mesh = new OpenGLMesh();
	mesh->shader_name = doc["shader"].GetString();

	// Skip the vertex format/shader for now
	// (This is changed in a later chapter's code)
	size_t vertSize = 8;

	// Load textures
	const rapidjson::Value& textures = doc["textures"];
	if ( !textures.IsArray() || textures.Size() < 1 )
	{
		std::ostringstream s;
		s << "Mesh " << path << " has no textures, there should be at least one";
		Logger::error( s.str() );
	}

	//mesh.setSpecularPower( doc["specularPower"].GetFloat() );

	for ( rapidjson::SizeType i = 0; i < textures.Size(); i++ )
	{
		std::string texName = textures[i].GetString();
		Texture* texture = get_texture( texName, {}, append_resources_path );
		mesh->add_texture( texture );
	}

	// Load in the vertices
	const rapidjson::Value& vertsJson = doc["vertices"];
	if ( !vertsJson.IsArray() || vertsJson.Size() < 1 )
	{
		std::ostringstream s;
		s << "Mesh " << path << " has no vertices";
		Logger::error( s.str() );
	}

	std::vector<float> vertices;
	vertices.reserve( vertsJson.Size() * vertSize );
	//float radius = 0.0f;
	for ( rapidjson::SizeType i = 0; i < vertsJson.Size(); i++ )
	{
		// For now, just assume we have 8 elements
		const rapidjson::Value& vert = vertsJson[i];
		if ( !vert.IsArray() || vert.Size() != 8 )
		{
			std::ostringstream s;
			s << "Unexpected vertex format for " << path;
			Logger::error( s.str() );
		}

		Vec3 pos( vert[0].GetFloat(), vert[1].GetFloat(), vert[2].GetFloat() );
		//radius = math::max( radius, pos.lengthSq() );

		// Add the floats
		for ( rapidjson::SizeType i = 0; i < vert.Size(); i++ )
		{
			vertices.emplace_back( vert[i].GetFloat() );
		}
	}

	// We were computing length squared earlier
	//mesh.setRadius( math::sqrt( radius ) );

	// Load in the indices
	const rapidjson::Value& indJson = doc["indices"];
	if ( !indJson.IsArray() || indJson.Size() < 1 )
	{
		std::ostringstream s;
		s << "Mesh " << path << " has no indices";
		Logger::error( s.str() );
	}

	std::vector<unsigned int> indices;
	indices.reserve( (size_t) ( indJson.Size() * 3.0f ) );
	for ( rapidjson::SizeType i = 0; i < indJson.Size(); i++ )
	{
		const rapidjson::Value& ind = indJson[i];
		if ( !ind.IsArray() || ind.Size() != 3 )
		{
			std::ostringstream s;
			s << "Invalid indices for " << path;
			Logger::error( s.str() );
		}

		indices.emplace_back( ind[0].GetUint() );
		indices.emplace_back( ind[1].GetUint() );
		indices.emplace_back( ind[2].GetUint() );
	}

	// Now create a vertex array
	mesh->vertex_array = new VertexArray(
		vertices.data(),
		static_cast<unsigned int>( vertices.size() ) / vertSize,
		indices.data(),
		static_cast<unsigned int>( indices.size() )
	);

	Logger::info( "Loaded mesh " + path );
	return mesh;
}
