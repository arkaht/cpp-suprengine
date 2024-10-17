#include "shader.h"

#include <sstream>
#include <string>
#include <vector>

#include <SDL.h>
#include <gl/glew.h>

#include <suprengine/logger.h>
#include <suprengine/assert.hpp>

using namespace suprengine;

void print_shader_info_log( uint32 shader_id )
{
	int log_length = 0;
	glGetShaderiv( shader_id, GL_INFO_LOG_LENGTH, &log_length );

	std::string info_log;
	info_log.reserve( log_length );
	glGetShaderInfoLog( shader_id, log_length, &log_length, info_log.data() );

	Logger::info( "Shader (ID: %d) info log: %s", shader_id, *info_log );
}

void print_program_info_log( uint32 program_id )
{
	int log_length = 0;
	glGetProgramiv( program_id, GL_INFO_LOG_LENGTH, &log_length );

	std::string info_log;
	info_log.reserve( log_length );
	glGetProgramInfoLog( program_id, log_length, &log_length, info_log.data() );

	Logger::info( "Program (ID: %d) info log: %s", program_id, *info_log );
}

void try_attach_shader( uint32 program_id, uint32 shader_id )
{
	if ( shader_id == 0 ) return;
	glAttachShader( program_id, shader_id );
}

void try_detach_shader( uint32 program_id, uint32 shader_id )
{
	if ( shader_id == 0 ) return;
	glDetachShader( program_id, shader_id );
}

void try_delete_shader( uint32 shader_id )
{
	if ( shader_id == 0 ) return;
	glDeleteShader( shader_id );
}

Shader::Shader(
	const char* vertex_code,
	const char* fragment_code,
	const char* tess_control_code,
	const char* tess_eval_code,
	const char* geometry_code
)
{
	ASSERT( strlen( vertex_code ) > 0, "Vertex shader code is empty" );
	ASSERT( strlen( fragment_code ) > 0, "Fragment shader code is empty" );

	uint32 vertex_shader_id = _create_shader(
		GL_VERTEX_SHADER, vertex_code, "vertex"
	);
	ASSERT( vertex_shader_id != 0, "Vertex shader failed to compile" );

	uint32 fragment_shader_id = _create_shader(
		GL_FRAGMENT_SHADER, fragment_code, "fragment"
	);
	ASSERT( fragment_shader_id != 0, "Fragment shader failed to compile" );

	uint32 tess_control_shader_id = _create_shader(
		GL_TESS_CONTROL_SHADER, tess_control_code, "tessellation control"
	);
	uint32 tess_evaluation_shader_id = _create_shader(
		GL_TESS_EVALUATION_SHADER, tess_eval_code, "tessellation evaluation"
	);
	uint32 geometry_shader_id = _create_shader(
		GL_GEOMETRY_SHADER, geometry_code, "geometry"
	);

	_create_program(
		vertex_shader_id,
		fragment_shader_id,
		tess_control_shader_id,
		tess_evaluation_shader_id,
		geometry_shader_id
	);

	print_all_params();
}

Shader::~Shader()
{
	if ( _program_id != 0 )
	{
		glDeleteProgram( _program_id );
	}
}

void Shader::activate()
{
	glUseProgram( _program_id );
}

bool Shader::is_valid() const
{
	return _program_id != 0;
}

const char* gl_type_to_string( GLenum type )
{
	switch ( type )
	{
		case GL_BOOL:
			return "bool";
		case GL_INT:
			return "int";
		case GL_FLOAT:
			return "float";
		case GL_FLOAT_VEC2:
			return "vec2";
		case GL_FLOAT_VEC3:
			return "vec3";
		case GL_FLOAT_VEC4:
			return "vec4";
		case GL_FLOAT_MAT2:
			return "mat2";
		case GL_FLOAT_MAT3:
			return "mat3";
		case GL_FLOAT_MAT4:
			return "mat4";
		case GL_SAMPLER_2D:
			return "sampler2D";
		case GL_SAMPLER_3D:
			return "sampler3D";
		case GL_SAMPLER_CUBE:
			return "samplerCube";
		case GL_SAMPLER_2D_SHADOW:
			return "sampler2DShadow";
		default:
			break;
	}

	return "N/A";
}

void Shader::print_all_params() const
{
	Logger::info( "Program (ID: %d) parameters:", _program_id );

	int params = -1;
	glGetProgramiv( _program_id, GL_LINK_STATUS, &params );
	Logger::info( "GL_LINK_STATUS = %d", params );

	glGetProgramiv( _program_id, GL_ATTACHED_SHADERS, &params );
	Logger::info( "GL_ATTACHED_SHADERS = %d", params );

	glGetProgramiv( _program_id, GL_ACTIVE_ATTRIBUTES, &params );
	Logger::info( "GL_ACTIVE_ATTRIBUTES = %d", params );

	//	Print all attributes
	for ( int i = 0; i < params; i++ )
	{
		const int max_length = 64;
		char name[max_length];
		int size = 0;
		GLenum type;
		glGetActiveAttrib( _program_id, i, max_length, NULL, &size, &type, name );

		if ( size > 1 )
		{
			//	Prints arrays attributes
			for ( int j = 0; j < size; j++ )
			{
				char long_name[77];
				sprintf_s( long_name, "%s[%i]", name, j );
				int location = glGetAttribLocation( _program_id, long_name );
				Logger::info(
					" %d) %s %s (location: %d)",
					i, gl_type_to_string( type ), long_name, location
				);
			}
		}
		else
		{
			int location = glGetAttribLocation( _program_id, name );
			Logger::info(
				" %d) %s %s (location: %d)",
				i, gl_type_to_string( type ), name, location
			);
		}
	}

	glGetProgramiv( _program_id, GL_ACTIVE_UNIFORMS, &params );
	Logger::info( "GL_ACTIVE_UNIFORMS = %d", params );

	//	Print all uniforms
	for ( int i = 0; i < params; i++ )
	{
		const int max_length = 64;
		char name[max_length];
		int size = 0;
		GLenum type;
		glGetActiveUniform( _program_id, i, max_length, NULL, &size, &type, name );

		if ( size > 1 )
		{
			//	Prints arrays uniforms
			for ( int j = 0; j < size; j++ )
			{
				char long_name[77];
				sprintf_s( long_name, "%s[%i]", name, j );

				int location = glGetUniformLocation( _program_id, long_name );
				Logger::info(
					" %d) %s %s (location: %d)",
					i, gl_type_to_string( type ), long_name, location
				);
			}
		}
		else
		{
			int location = glGetUniformLocation( _program_id, name );
			Logger::info(
				" %d) %s %s (location: %d)",
				i, gl_type_to_string( type ), name, location
			);
		}
	}
}

uint32 Shader::_create_shader( uint32 type, const char* code, const char* name )
{
	if ( code == nullptr ) return 0;

	//	Create shader
	const uint32 shader_id = glCreateShader( type );
	glShaderSource( shader_id, 1, &code, NULL );

	//	Compile shader
	if ( !_compile_shader( shader_id, name ) )
	{
		//	Free as soon as possible the shader to avoid memory leak
		glDeleteShader( shader_id );
		return 0;
	}

	Logger::info( "Compiled %s shader (ID: %d)", name, shader_id );

	return shader_id;
}

void Shader::_create_program(
	uint32 vertex_shader_id,
	uint32 fragment_shader_id,
	uint32 tess_control_shader_id,
	uint32 tess_evaluation_shader_id,
	uint32 geometry_shader_id
)
{
	//	Create program
	_program_id = glCreateProgram();
	try_attach_shader( _program_id, vertex_shader_id );
	try_attach_shader( _program_id, fragment_shader_id );
	try_attach_shader( _program_id, tess_control_shader_id );
	try_attach_shader( _program_id, tess_evaluation_shader_id );
	try_attach_shader( _program_id, geometry_shader_id );

	//	Check for errors
	if ( !_link_and_validate_program() )
	{
		glDeleteProgram( _program_id );
		_program_id = 0;
	}
	else
	{
		Logger::info( "Linked and validated shader program (ID: %d)", _program_id );
	}

	//	Mark shaders to be deleted as soon as possible
	try_delete_shader( vertex_shader_id );
	try_delete_shader( fragment_shader_id );
	try_delete_shader( tess_control_shader_id );
	try_delete_shader( tess_evaluation_shader_id );
	try_delete_shader( geometry_shader_id );
}

void Shader::set_float( const char* name, float value )
{
	glUniform1f( glGetUniformLocation( _program_id, name ), value );
}

void Shader::set_int( const char* name, int value )
{
	glUniform1i( glGetUniformLocation( _program_id, name ), value );
}

void Shader::set_vec2( const char* name, float x, float y )
{
	glUniform2f( glGetUniformLocation( _program_id, name ), x, y );
}
void Shader::set_vec2( const char* name, const Vec2& value )
{
	glUniform2f( glGetUniformLocation( _program_id, name ), value.x, value.y );
}

void Shader::set_vec3( const char* name, float x, float y, float z )
{
	glUniform3f( glGetUniformLocation( _program_id, name ), x, y, z );
}
void Shader::set_vec3( const char* name, const Vec3& value )
{
	set_vec3( name, value.x, value.y, value.z );
}

void Shader::set_vec4( const char* name, float x, float y, float z, float w )
{
	glUniform4f( glGetUniformLocation( _program_id, name ), x, y, z, w );
}

void Shader::set_color( const char* name, const Color& value )
{
	set_vec4(
		name,
		value.r / 255.0f,
		value.g / 255.0f,
		value.b / 255.0f,
		value.a / 255.0f
	);
}

void Shader::set_mtx4( const char* name, const Mtx4& matrix )
{
	glUniformMatrix4fv( glGetUniformLocation( _program_id, name ), 1, GL_TRUE, matrix.get_as_float_pointer() );
}

bool Shader::_compile_shader( uint32 shader_id, const char* name )
{
	int status = GL_FALSE;

	glCompileShader( shader_id );
	glGetShaderiv( shader_id, GL_COMPILE_STATUS, &status );
	if ( status != GL_TRUE )
	{
		Logger::error(
			"Failed to compile %s shader (ID: %d), status %d",
			name, shader_id, status
		);
		print_shader_info_log( shader_id );
		return false;
	}

	return true;
}

bool Shader::_link_and_validate_program()
{
	int status = GL_FALSE;

	//	Linking program
	glLinkProgram( _program_id );
	glGetProgramiv( _program_id, GL_LINK_STATUS, &status );
	if ( status != GL_TRUE )
	{
		Logger::error(
			"Failed to link shaders to program (ID: %d), status %d",
			_program_id, status
		);
		print_program_info_log( _program_id );
		return false;
	}

	//	Validate program
	glValidateProgram( _program_id );
	glGetProgramiv( _program_id, GL_VALIDATE_STATUS, &status );
	if ( status != GL_TRUE )
	{
		Logger::error(
			"Failed to validate shader program (ID: %d), status %d",
			_program_id, status
		);
		print_program_info_log( _program_id );
		return false;
	}

	return true;
}