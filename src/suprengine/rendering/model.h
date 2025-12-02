#pragma once

#include "mesh.h"

namespace suprengine
{
	class Model
	{
	public:
		std::string shader_program_name;

		Model( Mesh* mesh, rconst_str shader_name )
			: _meshes( { mesh } ), shader_program_name( shader_name )
		{}
		Model( const std::vector<Mesh*>& meshes, rconst_str shader_name )
			: _meshes( meshes ), shader_program_name( shader_name )
		{}

		Mesh* get_mesh( int id ) { return _meshes[id]; }
		int get_mesh_count() { return (int)_meshes.size(); }

	private:
		std::vector<Mesh*> _meshes;
	};
}