#include "shader-program.h"

#include <gl/glew.h>

#include <suprengine/math/color.h>
#include <suprengine/math/vec2.h>
#include <suprengine/math/mtx4.h>

#include <suprengine/rendering/shader.h>

#include <suprengine/tools/memory-profiler.h>
#include <suprengine/tools/profiler.h>

using namespace suprengine;

namespace
{
	const char* gl_type_to_string( const GLenum type )
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

	std::string print_program_info_log( const GLuint id )
	{
		GLint log_length = 0;
		glGetProgramiv( id, GL_INFO_LOG_LENGTH, &log_length );

		std::string info_log;
		info_log.reserve( log_length );
		glGetProgramInfoLog( id, log_length, &log_length, info_log.data() );

		return info_log;
	}
}

ShaderProgram::ShaderProgram( const std::string& name, const std::vector<Shader>& shaders )
	: _name( name )
{
	_id = glCreateProgram();

	for ( const Shader& shader : shaders )
	{
		ASSERT( shader.is_valid() );

		glAttachShader( _id, shader.get_id() );
	}

	if ( !link() || !validate() )
	{
		glDeleteProgram( _id );
		_id = 0;
		return;
	}

	update_uniform_locations();
}

ShaderProgram::~ShaderProgram()
{
	if ( _id != 0 )
	{
		glDeleteProgram( _id );
	}
}

void ShaderProgram::activate()
{
	glUseProgram( _id );
}

bool ShaderProgram::prepare( const uint32 frame_tick )
{
	if ( frame_tick == _last_preparation_tick ) return false;

	_last_preparation_tick = frame_tick;
	return true;
}

void ShaderProgram::set_float( const char* name, const float value )
{
	glUniform1f( get_uniform_location( name ), value );
}

void ShaderProgram::set_int( const char* name, const int value )
{
	glUniform1i( get_uniform_location( name ), value );
}

void ShaderProgram::set_vec2( const char* name, const Vec2& value )
{
	glUniform2f( get_uniform_location( name ), value.x, value.y );
}

void ShaderProgram::set_vec3( const char* name, const Vec3& value )
{
	glUniform3f( get_uniform_location( name ), value.x, value.y, value.z );
}

void ShaderProgram::set_vec4(
	const char* name,
	const float x,
	const float y,
	const float z,
	const float w
)
{
	glUniform4f( get_uniform_location( name ), x, y, z, w );
}

void ShaderProgram::set_color( const char* name, const Color& value )
{
	set_vec4(
		name,
		static_cast<float>( value.r ) / 255.0f,
		static_cast<float>( value.g ) / 255.0f,
		static_cast<float>( value.b ) / 255.0f,
		static_cast<float>( value.a ) / 255.0f
	);
}

void ShaderProgram::set_mtx4( const char* name, const Mtx4& matrix )
{
	glUniformMatrix4fv( get_uniform_location( name ), 1, GL_TRUE, &matrix[0][0] );
}

void ShaderProgram::print_all_params() const
{
	Logger::info( "Shader program '%s' (ID: %d) parameters:", *_name, _id );

	int params = -1;
	glGetProgramiv( _id, GL_LINK_STATUS, &params );
	Logger::info( "GL_LINK_STATUS = %d", params );

	glGetProgramiv( _id, GL_ATTACHED_SHADERS, &params );
	Logger::info( "GL_ATTACHED_SHADERS = %d", params );

	glGetProgramiv( _id, GL_ACTIVE_ATTRIBUTES, &params );
	Logger::info( "GL_ACTIVE_ATTRIBUTES = %d", params );

	//	Print all attributes
	for ( int i = 0; i < params; i++ )
	{
		const int max_length = 64;
		char name[max_length];
		int size = 0;
		GLenum type;
		glGetActiveAttrib( _id, i, max_length, NULL, &size, &type, name );

		if ( size > 1 )
		{
			//	Prints arrays attributes
			for ( int j = 0; j < size; j++ )
			{
				static constexpr GLsizei MAX_ARRAY_NAME_LENGTH = 77;
				char long_name[MAX_ARRAY_NAME_LENGTH];
				std::snprintf( long_name, MAX_ARRAY_NAME_LENGTH, "%s[%d]", name, j );

				const GLint location = glGetAttribLocation( _id, long_name );
				Logger::info(
					" %d) %s %s (location: %d)",
					i, gl_type_to_string( type ), long_name, location
				);
			}
		}
		else
		{
			const GLint location = glGetAttribLocation( _id, name );
			Logger::info(
				" %d) %s %s (location: %d)",
				i, gl_type_to_string( type ), name, location
			);
		}
	}

	//	TODO: Use _uniform_locations
	glGetProgramiv( _id, GL_ACTIVE_UNIFORMS, &params );
	Logger::info( "GL_ACTIVE_UNIFORMS = %d", params );

	//	Print all uniforms
	for ( int i = 0; i < params; i++ )
	{
		static constexpr int MAX_NAME_LENGTH = 64;
		char name[MAX_NAME_LENGTH];
		int size = 0;
		GLenum type;
		glGetActiveUniform( _id, i, MAX_NAME_LENGTH, nullptr, &size, &type, name );

		if ( size > 1 )
		{
			//	Prints arrays uniforms
			for ( int j = 0; j < size; j++ )
			{
				char long_name[77];
				sprintf_s( long_name, "%s[%i]", name, j );

				const GLint location = glGetUniformLocation( _id, long_name );
				Logger::info(
					" %d) %s %s (location: %d)",
					i, gl_type_to_string( type ), long_name, location
				);
			}
		}
		else
		{
			const GLint location = glGetUniformLocation( _id, name );
			Logger::info(
				" %d) %s %s (location: %d)",
				i, gl_type_to_string( type ), name, location
			);
		}
	}
}

uint32 ShaderProgram::get_uniform_location( const char* name ) const
{
	PROFILE_SCOPE( "ShaderProgram::get_uniform_location" );

	const UniformsLocationsMap::const_iterator itr = _uniform_locations.find( name );
	if ( itr == _uniform_locations.end() ) return -1;

	return itr->second;
}

bool ShaderProgram::is_valid() const
{
	return _id != 0;
}

uint32 ShaderProgram::get_id() const
{
	return _id;
}

const std::string& ShaderProgram::get_name() const
{
	return _name;
}

#ifdef ENABLE_MEMORY_PROFILER
void* ShaderProgram::operator new( std::size_t bytes )
{
	return MemoryProfiler::allocate( "Shader", bytes );
}
#endif

bool ShaderProgram::link()
{
	glLinkProgram( _id );

	GLint status = GL_FALSE;
	glGetProgramiv( _id, GL_LINK_STATUS, &status );
	if ( status != GL_TRUE )
	{
		Logger::error(
			"Failed to link shaders to program (ID: %d), status %d",
			_id, status
		);

		const std::string info_log = print_program_info_log( _id );
		Logger::error( "%s", *info_log );
		return false;
	}

	return true;
}

bool ShaderProgram::validate()
{
	glValidateProgram( _id );

	GLint status = GL_FALSE;
	glGetProgramiv( _id, GL_VALIDATE_STATUS, &status );
	if ( status != GL_TRUE )
	{
		Logger::error(
			"Failed to validate shader program (ID: %d), status %d",
			_id, status
		);

		const std::string info_log = print_program_info_log( _id );
		Logger::error( "%s", *info_log );
		return false;
	}

	return true;
}

void ShaderProgram::update_uniform_locations()
{
	GLint count = 0;
	glGetProgramiv( _id, GL_ACTIVE_UNIFORMS, &count );
	_uniform_locations.reserve( count );

	for ( int id = 0; id < count; id++ )
	{
		static constexpr GLsizei MAX_NAME_LENGTH = 64;
		char name[MAX_NAME_LENGTH];
		int size = 0;
		GLenum type;
		glGetActiveUniform( _id, id, MAX_NAME_LENGTH, nullptr, &size, &type, name );

		if ( size > 1 )
		{
			//	Prints arrays uniforms
			for ( int j = 0; j < size; j++ )
			{
				static constexpr GLsizei MAX_ARRAY_NAME_LENGTH = 77;
				char long_name[MAX_ARRAY_NAME_LENGTH];
				std::snprintf( long_name, MAX_ARRAY_NAME_LENGTH, "%s[%d]", name, j );

				const GLint location = glGetUniformLocation( _id, long_name );
				_uniform_locations.emplace( long_name, location );
			}
		}
		else
		{
			const int location = glGetUniformLocation( _id, name );
			_uniform_locations.emplace( name, location );
		}
	}
}