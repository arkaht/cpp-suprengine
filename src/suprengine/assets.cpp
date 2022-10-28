#include "assets.h"

using namespace suprengine;

std::map<std::string, Texture*> Assets::textures;
RenderBatch* Assets::render_batch;

Texture* Assets::get_texture( const std::string& filename )
{
	//  load texture if un-found
	if ( textures.find( filename ) == textures.end() )
	{
		textures[filename] = Texture::load( render_batch, filename );
	}

	//  get from textures
	return textures[filename];
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
