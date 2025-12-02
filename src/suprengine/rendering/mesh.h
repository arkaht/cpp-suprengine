#pragma once

#include <suprengine/utils/usings.h>
#include <suprengine/utils/memory.h>

#include <suprengine/math/vec2.h>

#include <vector>

namespace suprengine
{
	class ShaderProgram;
	class VertexArray;
	class Texture;
}

namespace suprengine
{
	class Mesh
	{
	public:
		Mesh( VertexArray* vertex_array, rconst_str shader_program_name = "" );
		Mesh( const Mesh& mesh ) = delete;
		~Mesh();

		void operator=( const Mesh& mesh ) = delete;

		int add_texture( SharedPtr<Texture> texture );
		SharedPtr<Texture> get_texture( int id );

		SharedPtr<ShaderProgram> get_shader_program() const;
		VertexArray* get_vertex_array() const;

	public:
		std::string shader_program_name {};
		Vec2 tiling = Vec2::one;
		bool should_cull_faces = true;

	private:
		VertexArray* _vertex_array { nullptr };
		std::vector<SharedPtr<Texture>> _textures;
	};
}