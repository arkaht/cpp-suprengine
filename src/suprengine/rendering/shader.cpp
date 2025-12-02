#include "shader.h"

#include <gl/glew.h>

#include <suprengine/utils/assert.h>
#include <suprengine/utils/logger.h>

using namespace suprengine;

namespace
{
	GLenum get_shader_type( const ShaderType type )
	{
		switch ( type )
		{
			case ShaderType::Vertex:
				return GL_VERTEX_SHADER;
			case ShaderType::Fragment:
				return GL_FRAGMENT_SHADER;
			case ShaderType::Geometry:
				return GL_GEOMETRY_SHADER;
			case ShaderType::TessellationControl:
				return GL_TESS_CONTROL_SHADER;
			case ShaderType::TessellationEvaluation:
				return GL_TESS_EVALUATION_SHADER;
			case ShaderType::Compute:
				return GL_COMPUTE_SHADER;
		}

		ASSERT( false );
		return 0;
	}

	std::string get_shader_info_log( const GLuint shader_id )
	{
		GLint log_length = 0;
		glGetShaderiv( shader_id, GL_INFO_LOG_LENGTH, &log_length );

		std::string info_log;
		info_log.reserve( log_length );
		glGetShaderInfoLog( shader_id, log_length, &log_length, info_log.data() );

		return info_log;
	}
}

Shader::Shader( const std::string& name, const std::string& code, const ShaderType type ) :
	_name( name ), _code( code ), _type( type )
{
	ASSERT( !name.empty() );
	ASSERT( !code.empty() );

	const GLenum shader_type = get_shader_type( _type );
	_id	= glCreateShader( shader_type );

	const char* raw_code = _code.c_str();
	glShaderSource( _id, 1, &raw_code, nullptr );

	if ( !compile() )
	{
		glDeleteShader( _id );
		_id = 0;
	}
}

Shader::~Shader()
{
	if ( _id != 0 )
	{
		glDeleteShader( _id );
	}
}

bool Shader::is_valid() const
{
	return _id != 0;
}

uint32 Shader::get_id() const
{
	return _id;
}

const std::string& Shader::get_name() const
{
	return _name;
}

bool Shader::compile()
{
	glCompileShader( _id );

	GLint status = GL_FALSE;
	glGetShaderiv( _id, GL_COMPILE_STATUS, &status );
	if ( status != GL_TRUE )
	{
		Logger::error(
			"Failed to compile %s shader '%s' (ID: %d), status %d",
			get_shader_type_name( _type ), *_name, _id, status
		);

		const std::string info_log = get_shader_info_log( _id );
		Logger::error( "%s", *info_log );
		return false;
	}

	return true;
}
