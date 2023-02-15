#include "shader.h"

#include <SDL.h>
#include <sstream>
#include <string>

#include <suprengine/logger.h>

using namespace suprengine;

Shader::~Shader()
{
    glDeleteProgram( id );
}

void Shader::activate()
{
    glUseProgram( id );
}

void Shader::compile( const GLchar* vertexSource, const GLchar* fragmentSource,
    const GLchar* tessControlSource, const GLchar* tessEvalSource, const GLchar* geometrySource )
{
    compile_vertex_shader( vertexSource );
    bool tessExists = compile_tess_control_shader( tessControlSource );
    tessExists &= compile_tess_eval_shader( tessEvalSource );
    bool gsExists = compile_geometry_shader( geometrySource );
    compile_fragment_shader( fragmentSource );
    create_shader_program( tessExists, gsExists );
    print_all_params( id );
}

void Shader::compile_vertex_shader( const GLchar* vertex_source )
{
    vs = glCreateShader( GL_VERTEX_SHADER );
    glShaderSource( vs, 1, &vertex_source, NULL );
    glCompileShader( vs );
    check_shader_errors( vs, "vertex" );
}

void Shader::compile_fragment_shader( const GLchar* fragment_source )
{
    fs = glCreateShader( GL_FRAGMENT_SHADER );
    glShaderSource( fs, 1, &fragment_source, NULL );
    glCompileShader( fs );
    check_shader_errors( fs, "fragment" );
}

bool Shader::compile_tess_control_shader( const GLchar* tessControlSource )
{
    if ( tessControlSource == nullptr )
    {
        return false;
    }

    tcs = glCreateShader( GL_TESS_CONTROL_SHADER );
    glShaderSource( tcs, 1, &tessControlSource, NULL );
    glCompileShader( tcs );
    check_shader_errors( tcs, "tessellation control" );
    return true;
}

bool Shader::compile_tess_eval_shader( const GLchar* tessEvalSource )
{
    if ( tessEvalSource == nullptr )
    {
        return false;
    }

    tes = glCreateShader( GL_TESS_EVALUATION_SHADER );
    glShaderSource( tes, 1, &tessEvalSource, NULL );
    glCompileShader( tes );
    check_shader_errors( tes, "tessellation evaluation" );
    return true;
}

bool Shader::compile_geometry_shader( const GLchar* geometry_source )
{
    if ( geometry_source == nullptr )
    {
        return false;
    }

    gs = glCreateShader( GL_GEOMETRY_SHADER );
    glShaderSource( gs, 1, &geometry_source, NULL );
    glCompileShader( gs );
    check_shader_errors( gs, "geometry" );

    return true;
}

void Shader::create_shader_program( bool tessShadersExist, bool geometryShaderExists )
{
    // Create program
    id = glCreateProgram();
    glAttachShader( id, fs );
    if ( tessShadersExist )
    {
        glAttachShader( id, tcs );
        glAttachShader( id, tes );
    }
    if ( geometryShaderExists )
    {
        glAttachShader( id, gs );
    }
    glAttachShader( id, vs );
    glLinkProgram( id );

    // Check for linking error
    int params = -1;
    glGetProgramiv( id, GL_LINK_STATUS, &params );
    if ( params != GL_TRUE )
    {
        std::ostringstream s;
        s << "Could not link shader programme GL index " << id;
        Logger::error( LOG_CATEGORY::RENDER, s.str() );
        print_program_info_log( id );
    }
    if ( !is_valid( id ) )
    {
        std::ostringstream s;
        s << "Could not validate shader " << id;
        Logger::error( LOG_CATEGORY::RENDER, s.str() );
    }

    // Delete shaders for they are no longer used
    glDeleteShader( vs );
    if ( tessShadersExist )
    {
        glDeleteShader( tcs );
        glDeleteShader( tes );
    }
    if ( geometryShaderExists )
    {
        glDeleteShader( gs );
    }
    glDeleteShader( fs );
}

void Shader::set_float( const GLchar* name, GLfloat value )
{
    glUniform1f( glGetUniformLocation( id, name ), value );
}
void Shader::set_int( const GLchar* name, GLint value )
{
    glUniform1i( glGetUniformLocation( id, name ), value );
}
void Shader::set_vec2( const GLchar* name, GLfloat x, GLfloat y )
{
    glUniform2f( glGetUniformLocation( id, name ), x, y );
}
void Shader::set_vec2( const GLchar* name, const Vec2& value )
{
    glUniform2f( glGetUniformLocation( id, name ), value.x, value.y );
}

void Shader::set_vec3( const GLchar* name, GLfloat x, GLfloat y, GLfloat z )
{
    glUniform3f( glGetUniformLocation( id, name ), x, y, z );
}
void Shader::set_vec3( const GLchar* name, const Vec3& value )
{
    glUniform3f( glGetUniformLocation( id, name ), value.x, value.y, value.z );
}
/*
void Shader::setVector4f(const GLchar* name, GLfloat x, GLfloat y, GLfloat z, GLfloat w)
{
    glUniform4f(glGetUniformLocation(id, name), x, y, z, w);
}
void Shader::setVector4f(const GLchar* name, const Vector4& value)
{
    glUniform4f(glGetUniformLocation(id, name), value.x, value.y, value.z, value.w);
}
*/
void Shader::set_mtx4( const GLchar* name, const Mtx4& matrix )
{
    glUniformMatrix4fv( glGetUniformLocation( id, name ), 1, GL_TRUE, matrix.get_as_float_pointer() );
}

void Shader::print_shader_info_log( GLuint shaderIndex )
{
    int max_length = 2048;
    int actual_length = 0;
    char log[2048];
    glGetShaderInfoLog( shaderIndex, max_length, &actual_length, log );
    std::ostringstream s;
    s << "Shader info log for GL index " << shaderIndex;
    Logger::info( s.str() );
}

void Shader::print_program_info_log( GLuint id )
{
    int max_length = 2048;
    int actual_length = 0;
    char log[2048];
    glGetProgramInfoLog( id, max_length, &actual_length, log );
    std::ostringstream s;
    s << "Program info log for GL index " << id;
    Logger::info( s.str() );
}

void Shader::check_shader_errors( GLuint shader, std::string shaderType )
{
    int params = -1;
    glGetShaderiv( shader, GL_COMPILE_STATUS, &params );
    if ( params != GL_TRUE )
    {
        std::ostringstream s;
        s << "GL " << shaderType << " shader index " << shader << " did not compile.";
        Logger::error( LOG_CATEGORY::RENDER, s.str() );
        print_shader_info_log( shader );
    }
}

const char* Shader::gl_type_to_string( GLenum type )
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
    return "other";
}

void Shader::print_all_params( GLuint id )
{
    Logger::info( "-----------------------------" );
    std::ostringstream s;
    s << "Shader programme " << id << " info: ";
    Logger::info( s.str() );
    int params = -1;
    glGetProgramiv( id, GL_LINK_STATUS, &params );
    s.str( "" );
    s << "GL_LINK_STATUS = " << params;
    Logger::info( s.str() );

    glGetProgramiv( id, GL_ATTACHED_SHADERS, &params );
    s.str( "" );
    s << "GL_ATTACHED_SHADERS = " << params;
    Logger::info( s.str() );

    glGetProgramiv( id, GL_ACTIVE_ATTRIBUTES, &params );
    s.str( "" );
    s << "GL_ACTIVE_ATTRIBUTES = " << params;
    Logger::info( s.str() );
    for ( GLuint i = 0; i < (GLuint) params; i++ )
    {
        char name[64];
        int max_length = 64;
        int actual_length = 0;
        int size = 0;
        GLenum type;
        glGetActiveAttrib( id, i, max_length, &actual_length, &size, &type, name );
        if ( size > 1 )
        {
            for ( int j = 0; j < size; j++ )
            {
                char long_name[77];
                sprintf_s( long_name, "%s[%i]", name, j );
                int location = glGetAttribLocation( id, long_name );
                std::ostringstream s;
                s << "  " << i << ") type:" << gl_type_to_string( type ) << " name:" << long_name << " location:" << location;
                Logger::info( s.str() );
            }
        }
        else
        {
            int location = glGetAttribLocation( id, name );
            std::ostringstream s;
            s << "  " << i << ") type:" << gl_type_to_string( type ) << " name:" << name << " location:" << location;
            Logger::info( s.str() );
        }
    }

    glGetProgramiv( id, GL_ACTIVE_UNIFORMS, &params );
    s.str( "" );
    s << "GL_ACTIVE_UNIFORMS = " << params;
    Logger::info( s.str() );
    for ( GLuint i = 0; i < (GLuint) params; i++ )
    {
        char name[64];
        int max_length = 64;
        int actual_length = 0;
        int size = 0;
        GLenum type;
        glGetActiveUniform( id, i, max_length, &actual_length, &size, &type, name );
        if ( size > 1 )
        {
            for ( int j = 0; j < size; j++ )
            {
                char long_name[77];
                sprintf_s( long_name, "%s[%i]", name, j );
                int location = glGetUniformLocation( id, long_name );
                std::ostringstream s;
                s << "  " << i << ") type:" << gl_type_to_string( type ) << " name:" << long_name << " location:" << location;
                Logger::info( s.str() );
            }
        }
        else
        {
            int location = glGetUniformLocation( id, name );
            std::ostringstream s;
            s << "  " << i << ") type:" << gl_type_to_string( type ) << " name:" << name << " location:" << location;
            Logger::info( s.str() );
        }
    }
    print_program_info_log( id );
}

bool Shader::is_valid( GLuint id )
{
    glValidateProgram( id );
    int params = -1;
    glGetProgramiv( id, GL_VALIDATE_STATUS, &params );
    Logger::info( "" );
    std::ostringstream s;
    s << "program " << id << " GL_VALIDATE_STATUS = " << params;
    Logger::info( s.str() );
    if ( params != GL_TRUE )
    {
        print_program_info_log( id );
        return false;
    }
    return true;
}