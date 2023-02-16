#include "vertex-array.h"

VertexArray::VertexArray( const float* vertices, unsigned int vertices_count, const unsigned int* indices, unsigned int indices_count )
	: vertices_count( vertices_count ), indices_count( indices_count )
{
	//  create vao
	glGenVertexArrays( 1, &vao_id );
	glBindVertexArray( vao_id );

	//  create vbo
	glGenBuffers( 1, &vbo_id );
	glBindBuffer( GL_ARRAY_BUFFER, vbo_id );
	glBufferData( GL_ARRAY_BUFFER, vertices_count * 8 * sizeof( float ), vertices, GL_STATIC_DRAW );

	//  create ibo
	glGenBuffers( 1, &ibo_id );
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, ibo_id );
	glBufferData( GL_ELEMENT_ARRAY_BUFFER, indices_count * sizeof( unsigned int ), indices, GL_STATIC_DRAW );

	//  set vertex attributes
	glEnableVertexAttribArray( 0 );  //  position
	glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, sizeof( float ) * 8, 0 );
	glEnableVertexAttribArray( 1 );  //  normal
	glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, sizeof( float ) * 8, reinterpret_cast<void*>( sizeof( float ) * 3 ) );
	glEnableVertexAttribArray( 2 );  //  uv
	glVertexAttribPointer( 2, 2, GL_FLOAT, GL_FALSE, sizeof( float ) * 8, reinterpret_cast<void*>( sizeof( float ) * 6 ) );
}

VertexArray::~VertexArray()
{
	glDeleteBuffers( 1, &vao_id );
	glDeleteBuffers( 1, &vbo_id );
	glDeleteBuffers( 1, &ibo_id );
}

void VertexArray::activate()
{
	glBindVertexArray( vao_id );
}
