#pragma once
#include <suprengine/mesh.hpp>
#include <suprengine/assets.cpp>

#include "vertex-array.h"
#include "opengl-texture.hpp"
#include "shader.h"

namespace suprengine
{
	class OpenGLMesh : Mesh
	{
	private:
	public:
		Shader* shader { nullptr };
		VertexArray* vertex_array { nullptr };

		OpenGLMesh( VertexArray* vertex_array )
			: vertex_array( vertex_array ), Mesh()
		{
			shader = Assets::get_shader( "simple-mesh" );
		}
		OpenGLMesh( VertexArray* vertex_array, Shader* shader )
			: vertex_array( vertex_array ), shader( shader ), Mesh() {}
		~OpenGLMesh()
		{
			delete vertex_array;
		}

		Shader* get_shader() override
		{
			return shader;
		}
		int get_indices_count() override
		{
			return vertex_array->get_indices_count();
		}

		void activate( int texture_id = 0 ) override
		{
			vertex_array->activate();

			if ( Texture* texture = get_texture( texture_id ) )
				texture->activate();

			shader->activate();
		}
	};
}