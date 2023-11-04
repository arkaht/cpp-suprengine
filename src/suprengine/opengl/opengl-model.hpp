#pragma once

#include "opengl-mesh.hpp"

namespace suprengine
{
	class Model
	{
	public:
		Model( std::vector<Mesh*>&& meshes )
			: _meshes( meshes )
		{}

		Mesh* get_mesh( int id ) { return _meshes[id]; }
		int get_mesh_count() { return (int)_meshes.size(); }

	private:
		std::vector<Mesh*> _meshes;
	};
}