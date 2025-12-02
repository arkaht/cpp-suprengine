#include "mesh.h"

#include <suprengine/core/assets.h>

#include <suprengine/rendering/vertex-array.h>
#include <suprengine/rendering/shader.h>

using namespace suprengine;

Mesh::Mesh( VertexArray* vertex_array, rconst_str shader_program_name )
	: _vertex_array( vertex_array ), shader_program_name( shader_program_name )
{}

Mesh::~Mesh()
{
	if ( _vertex_array != nullptr )
	{
		delete _vertex_array;
	}
}

int Mesh::add_texture( SharedPtr<Texture> texture )
{
	_textures.emplace_back( texture );
	return (int)_textures.size() - 1;
}

SharedPtr<Texture> Mesh::get_texture( int id )
{
	if ( id < 0 || id >= _textures.size() ) return nullptr;
	return _textures[id];
}

SharedPtr<ShaderProgram> Mesh::get_shader_program() const
{
	return Assets::get_shader_program( shader_program_name );
}

VertexArray* Mesh::get_vertex_array() const
{
	return _vertex_array;
}