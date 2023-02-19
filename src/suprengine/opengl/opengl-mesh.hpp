#pragma once
#include <suprengine/mesh.h>
#include <suprengine/assets.h>

#include "vertex-array.h"
#include "opengl-texture.hpp"
#include "shader.h"

namespace suprengine
{
	class OpenGLMesh : public Mesh
	{
	private:
	public:
		VertexArray* vertex_array { nullptr };

		OpenGLMesh() 
			: Mesh() {}
		OpenGLMesh( VertexArray* vertex_array ) 
			: vertex_array( vertex_array ), Mesh() {}
		OpenGLMesh( VertexArray* vertex_array, rconst_str shader_name )
			: vertex_array( vertex_array ), Mesh( shader_name ) {}
		~OpenGLMesh()
		{
			delete vertex_array;
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

			if ( Shader* shader = get_shader() )
				shader->activate();
		}
	};
}