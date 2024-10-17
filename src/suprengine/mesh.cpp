#include "mesh.h"

#include <suprengine/assets.h>
#include <suprengine/vertex-array.h>
#include <suprengine/shader.h>

using namespace suprengine;

Mesh::Mesh( VertexArray* vertex_array, rconst_str shader_name )
	: _vertex_array( vertex_array ), shader_name( shader_name )
{}

Mesh::~Mesh()
{
	delete _vertex_array;
}

int Mesh::add_texture( Texture* texture )
{
	_textures.emplace_back( texture );
	return (int)_textures.size() - 1;
}

Texture* Mesh::get_texture( int id )
{
	if ( id < 0 || id >= _textures.size() ) return nullptr;
	return _textures[id];
}

Shader* Mesh::get_shader() const
{
	return Assets::get_shader( shader_name );
}

VertexArray* Mesh::get_vertex_array() const
{
	return _vertex_array;
}