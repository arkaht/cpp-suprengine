#pragma once

#include <suprengine/usings.h>

#include <vector>

namespace suprengine
{
	class VertexArray;
	class Shader;
	class Texture;
	
	class Mesh
	{
	public:
		Mesh( VertexArray* vertex_array, rconst_str shader_name = "" );
		Mesh( const Mesh& mesh ) = delete;
		~Mesh();

		void operator=( const Mesh& mesh ) = delete;

		int add_texture( Texture* texture );
		Texture* get_texture( int id );

		Shader* get_shader() const;
		VertexArray* get_vertex_array() const;

	public:
		std::string shader_name {};

	private:
		VertexArray* _vertex_array { nullptr };
		std::vector<Texture*> _textures;
	};
}