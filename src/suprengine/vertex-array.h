#pragma once

#include <gl/glew.h>

namespace suprengine
{
	constexpr float QUAD_VERTICES[] = {
		//  Top-left
		0.0f, 0.0f, 0.0f,  //  Position
		0.0f, 0.0f, 0.0f,  //  Normal
			  0.0f, 0.0f,  //  UV
		//  Top-right
		1.0f, 0.0f, 0.0f,  //  Position
		0.0f, 0.0f, 0.0f,  //  Normal
			  1.0f, 0.0f,  //  UV
		//  Bottom-right
		1.0f, 1.0f, 0.0f,  //  Position
		0.0f, 0.0f, 0.0f,  //  Normal
			  1.0f, 1.0f,  //  UV
		//  Bottom-left
		0.0f, 1.0f, 0.0f,  //  Position
		0.0f, 0.0f, 0.0f,  //  Normal
			  0.0f, 1.0f   //  UV
	};

	constexpr unsigned int QUAD_INDICES[] = {
		0, 1, 2,
		2, 3, 0
	};

	/*
	 * A preset which define how a VertexArray should be handled
	 * by OpenGL.
	 */
	class VertexArrayPreset
	{
	public:
		static const VertexArrayPreset Position3_Normal3_UV2;
		static const VertexArrayPreset Position2_UV2;

	public:
		VertexArrayPreset( 
			unsigned int positions,
			unsigned int normals,
			unsigned int uvs
		);

	public:
		unsigned int stride;
		unsigned int positions;
		unsigned int normals;
		unsigned int uvs;
	};

	class VertexArray
	{
	public:
		VertexArray( 
			const VertexArrayPreset& preset,
			const float* vertices, 
			unsigned int vertices_count, 
			const unsigned int* indices, 
			unsigned int indices_count
		);
		~VertexArray();

		void activate();

		unsigned int get_vertices_count() const { return vertices_count; }
		unsigned int get_indices_count() const { return indices_count; }
	
	private:
		unsigned int vertices_count { 0 };
		unsigned int indices_count { 0 };
		unsigned int vao_id { 0 };
		unsigned int vbo_id { 0 };
		unsigned int ibo_id { 0 };
	};
}