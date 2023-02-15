#include "assets.h"

#include <iostream>
#include <sstream>
#include <fstream>

using namespace suprengine;

std::map<std::string, Texture*> Assets::textures;
std::map<std::string, Font*> Assets::fonts;
std::map<std::string, Shader*> Assets::shaders;

RenderBatch* Assets::render_batch { nullptr };
std::string Assets::path { "" };

Texture* Assets::get_texture( rconst_str filename )
{
	//  load texture if un-found
	if ( textures.find( filename ) == textures.end() )
	{
		textures[filename] = Texture::load( render_batch, path + filename );
	}

	//  get from textures
	return textures[filename];
}

Font* Assets::get_font( rconst_str filename, int size )
{
	std::string key = filename + std::to_string( size );

	//  load texture if un-found
	if ( fonts.find( key ) == fonts.end() )
	{
		fonts[key] = Font::load( path + filename, size );
	}

	//  get from textures
	return fonts[key];
}

Shader* Assets::load_shader( rconst_str name, rconst_str vtx_filename, rconst_str frg_filename, rconst_str tsc_filename, rconst_str tse_filename, rconst_str geo_filename )
{
	shaders[name] = load_shader_from_file( vtx_filename, frg_filename, tsc_filename, tse_filename, geo_filename );
	return get_shader( name );
}

Shader* Assets::get_shader( rconst_str name ) { return shaders[name]; }

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
        Logger::error( LOG_CATEGORY::RENDER, loadError.str() );
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
