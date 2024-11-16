#include "vertex-array.h"

#include <suprengine/logger.h>

#include <gl/glew.h>

using namespace suprengine;

/*
 * VertexArrayPreset
 */
const VertexArrayPreset VertexArrayPreset::Position3( 3, 0, 0 );
const VertexArrayPreset VertexArrayPreset::Position3_Normal3_UV2( 3, 3, 2 );
const VertexArrayPreset VertexArrayPreset::Position2_UV2( 2, 0, 2 );

/*
 * VertexArray
 */
VertexArray::VertexArray( 
	const VertexArrayPreset& preset,
	const float* vertices, 
	uint32 vertices_count, 
	const uint32* indices, 
	uint32 indices_count 
)
	: _vertices_count( vertices_count ), 
	  _indices_count( indices_count )
{
	//	Create vertex array object
	glGenVertexArrays( 1, &_vao_id );
	glBindVertexArray( _vao_id );

	//	Create vertex buffer object
	glGenBuffers( 1, &_vbo_id );
	glBindBuffer( GL_ARRAY_BUFFER, _vbo_id );
	glBufferData(
		GL_ARRAY_BUFFER,
		vertices_count * preset.stride * sizeof( float ),
		vertices,
		GL_STATIC_DRAW
	);

	//	Create indices buffer object
	if ( indices != nullptr && indices_count > 0 )
	{
		glGenBuffers( 1, &_ibo_id );
		glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, _ibo_id );
		glBufferData(
			GL_ELEMENT_ARRAY_BUFFER,
			indices_count * sizeof( uint32 ),
			indices,
			GL_STATIC_DRAW
		);
	}

	const GLsizei byte_stride = sizeof( float ) * preset.stride;
	
	uint32 attribute = 0;
	uint32 offset = 0;

	//	Positions
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

	//	Normals
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

	//	UVs
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

	Logger::info(
		"Created vertex array (ID: %d) with associated buffers (VBO: %d; IBO: %d), "
		"preset (P: %d; N: %d; UV: %d) "
		"and data (V: %d; I: %d)",
		_vao_id, _vbo_id, _ibo_id,
		preset.positions, preset.normals, preset.uvs,
		vertices_count, indices_count
	);
}

VertexArray::~VertexArray()
{
	glDeleteBuffers( 1, &_vao_id );
	glDeleteBuffers( 1, &_vbo_id );
	glDeleteBuffers( 1, &_ibo_id );
}

void VertexArray::activate()
{
	glBindVertexArray( _vao_id );
}

uint32 VertexArray::get_vertices_count() const { return _vertices_count; }
uint32 VertexArray::get_indices_count() const { return _indices_count; }
