#include "assets.h"

using namespace suprengine;

std::map<std::string, Texture*> Assets::textures;
std::map<std::string, Font*> Assets::fonts;

RenderBatch* Assets::render_batch { nullptr };
std::string Assets::path { "" };

Texture* Assets::get_texture( const std::string& filename )
{
	//  load texture if un-found
	if ( textures.find( filename ) == textures.end() )
	{
		textures[filename] = Texture::load( render_batch, path + filename );
	}

	//  get from textures
	return textures[filename];
}

Font* Assets::get_font( const std::string& filename, int size )
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

void Assets::release()
{
	//  release textures
	for ( auto& pair : textures )
	{
		Texture* texture = pair.second;
		if ( texture == nullptr ) continue;

		delete texture;
	}
	textures.clear();
}
