#pragma once

#include <suprengine/usings.h>
#include <suprengine/vertex-array.h>
#include <suprengine/shader.h>
#include <suprengine/texture.fwd.h>

#include <vector>

namespace suprengine
{
	class Mesh
	{
	public:
		std::string shader_name;

		Mesh() {}
		Mesh( VertexArray* _vertex_array )
			: _vertex_array( _vertex_array ) {}
		Mesh( VertexArray* _vertex_array, rconst_str shader_name )
			: _vertex_array( _vertex_array ) {}
		Mesh( const Mesh& mesh ) = delete;
		void operator=( const Mesh& mesh ) = delete;
		~Mesh();

		int add_texture( Texture* texture );
		Texture* get_texture( int id );

		Shader* get_shader();
		int get_indices_count();

		VertexArray* get_vertex_array() const { return _vertex_array; }

	private:
		VertexArray* _vertex_array { nullptr };
		std::vector<Texture*> _textures;
	};
}