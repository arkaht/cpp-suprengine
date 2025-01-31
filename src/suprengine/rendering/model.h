#pragma once

#include "mesh.h"

namespace suprengine
{
	class Model
	{
	public:
		std::string shader_name;

		Model( std::vector<Mesh*>&& meshes, std::string shader_name )
			: _meshes( meshes ), shader_name( shader_name )
		{}

		Mesh* get_mesh( int id ) { return _meshes[id]; }
		int get_mesh_count() { return (int)_meshes.size(); }

	private:
		std::vector<Mesh*> _meshes;
	};
}