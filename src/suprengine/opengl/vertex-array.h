#pragma once

#include <gl/glew.h>

constexpr float vertices[] = {
	//  top-left
	0.0f, 0.0f, 0.0f,  //  position
	0.0f, 0.0f, 0.0f,  //  normal
		  0.0f, 0.0f,  //  uv
	//  top-right
	1.0f, 0.0f, 0.0f,  //  position
	0.0f, 0.0f, 0.0f,  //  normal
		  1.0f, 0.0f,  //  uv
	//  bottom-right
	1.0f, 1.0f, 0.0f,  //  position
	0.0f, 0.0f, 0.0f,  //  normal
		  1.0f, 1.0f,  //  uv
	//  bottom-left
	0.0f, 1.0f, 0.0f,  //  position
	0.0f, 0.0f, 0.0f,  //  normal
		  0.0f, 1.0f   //  uv
};

constexpr unsigned int indices[] = {
	0, 1, 2,
	2, 3, 0
};

class VertexArray
{
private:
	unsigned int vertices_count { 0 }, indices_count { 0 };
	unsigned int vao_id { 0 }, vbo_id { 0 }, ibo_id { 0 };
public:
	VertexArray( const float* vertices, unsigned int vertices_count, const unsigned int* indices, unsigned int indices_count );
	~VertexArray();

	void activate();

	unsigned int get_vertices_count() const { return vertices_count; }
	unsigned int get_indices_count() const { return indices_count; }
};