#include "vertex-array.h"

using namespace suprengine;

/*
 * VertexArrayPreset
 */
const VertexArrayPreset 
	VertexArrayPreset::Position3_Normal3_UV2( 3, 3, 2 );
const VertexArrayPreset 
	VertexArrayPreset::Position2_UV2( 2, 0, 2 );

VertexArrayPreset::VertexArrayPreset( 
	unsigned int positions, 
	unsigned int normals, 
	unsigned int uvs 
)
	: positions( positions ),
	  normals( normals ),
	  uvs( uvs )
{
	stride = positions + normals + uvs;
}

/*
 * VertexArray
 */
VertexArray::VertexArray( 
	const VertexArrayPreset& preset,
	const float* vertices, 
	unsigned int vertices_count, 
	const unsigned int* indices, 
	unsigned int indices_count 
)
	: vertices_count( vertices_count ), 
	  indices_count( indices_count )
{
	//  create vao
	glGenVertexArrays( 1, &vao_id );
	glBindVertexArray( vao_id );

	//  create vbo
	glGenBuffers( 1, &vbo_id );
	glBindBuffer( GL_ARRAY_BUFFER, vbo_id );
	glBufferData( 
		GL_ARRAY_BUFFER, 
		vertices_count * preset.stride * sizeof( float ), 
		vertices, 
		GL_STATIC_DRAW 
	);

	//  create ibo
	glGenBuffers( 1, &ibo_id );
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, ibo_id );
	glBufferData( 
		GL_ELEMENT_ARRAY_BUFFER, 
		indices_count * sizeof( unsigned int ), 
		indices, 
		GL_STATIC_DRAW 
	);

	const GLsizei byte_stride = sizeof( float ) * preset.stride;
	
	int attribute = 0;
	int offset = 0;

	//  positions
	glEnableVertexAttribArray( attribute );
	glVertexAttribPointer(
		attribute, 
		preset.positions, 
		GL_FLOAT, GL_FALSE, 
		byte_stride, 
		0 
	);
	offset += preset.positions;
	attribute++;

	//  normals
	if ( preset.normals > 0 )
	{
		glEnableVertexAttribArray( attribute );
		glVertexAttribPointer( 
			attribute, 
			preset.normals, 
			GL_FLOAT, GL_FALSE, 
			byte_stride, 
			reinterpret_cast<void*>( sizeof( float ) * offset ) 
		);
		offset += preset.normals;
		attribute++;
	}

	//  uvs
	if ( preset.uvs > 0 )
	{
		glEnableVertexAttribArray( attribute );
		glVertexAttribPointer( 
			attribute, 
			preset.uvs, 
			GL_FLOAT, GL_FALSE, 
			byte_stride, 
			reinterpret_cast<void*>( sizeof( float ) * offset ) 
		);
		offset += preset.uvs;
		attribute++;
	}
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
