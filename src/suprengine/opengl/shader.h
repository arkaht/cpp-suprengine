#pragma once 
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include <iostream>
#include <string>
#include <fstream>

#include <suprengine/color.h>
#include <suprengine/vec2.h>
#include <suprengine/mtx4.h>

namespace suprengine
{
	class Shader
	{
	public:
		// State
		GLuint id;

		// Constructor
		Shader() : id( 0 ), vs( 0 ), fs( 0 ), gs( 0 ), tcs( 0 ), tes( 0 ) {}
		~Shader();

		// Sets the current shader as active
		void activate();

		// Compiles the shader from given source code
		void compile(
			const GLchar* vertex_source,
			const GLchar* fragment_source,
			const GLchar* tess_control_source = nullptr,
			const GLchar* tess_eval_source = nullptr,
			const GLchar* geometry_source = nullptr
		);

		// Utility functions
		void set_float( const GLchar* name, GLfloat value );
		void set_int( const GLchar* name, GLint value );
		void set_vec2( const GLchar* name, GLfloat x, GLfloat y );
		void set_vec2( const GLchar* name, const Vec2& value );
		void set_vec3( const GLchar* name, GLfloat x, GLfloat y, GLfloat z );
		void set_vec3( const GLchar* name, const Vec3& value );
		void set_vec4( const GLchar* name, GLfloat x, GLfloat y, GLfloat z, GLfloat w );
		void set_vec4( const GLchar* name, const Color& value );
		void set_mtx4( const GLchar* name, const Mtx4& matrix );

	private:
		GLuint vs;
		GLuint fs;
		GLuint tcs;
		GLuint tes;
		GLuint gs;

		void compile_vertex_shader( const GLchar* vertexSource );
		void compile_fragment_shader( const GLchar* fragmentSource );
		bool compile_tess_control_shader( const GLchar* tessControlSource );
		bool compile_tess_eval_shader( const GLchar* tessEvalSource );
		bool compile_geometry_shader( const GLchar* geometrySource );
		void create_shader_program( bool tessShadersExist, bool geometryShaderExists );

		void check_shader_errors( GLuint shader, std::string shaderType );
		void print_shader_info_log( GLuint shaderIndex );
		void print_program_info_log( GLuint program );
		const char* gl_type_to_string( GLenum type );
		void print_all_params( GLuint program );
		bool is_valid( GLuint program );
	};
}