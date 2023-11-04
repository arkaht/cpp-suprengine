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
	public:

		OpenGLMesh() 
			: Mesh() {}
		OpenGLMesh( VertexArray* _vertex_array ) 
			: _vertex_array( _vertex_array ), Mesh() {}
		OpenGLMesh( VertexArray* _vertex_array, rconst_str shader_name )
			: _vertex_array( _vertex_array ), Mesh( shader_name ) {}
		~OpenGLMesh()
		{
			delete _vertex_array;
		}

		int get_indices_count() override
		{
			return _vertex_array->get_indices_count();
		}

		void activate( int texture_id = 0 ) override
		{
			_vertex_array->activate();

			if ( Texture* texture = get_texture( texture_id ) )
				texture->activate();

			if ( Shader* shader = get_shader() )
				shader->activate();
		}

		VertexArray* _vertex_array;
	private:
	
	};
}