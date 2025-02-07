#pragma once

#include <suprengine/utils/usings.h>

#include <suprengine/math/color.h>
#include <suprengine/math/vec2.h>
#include <suprengine/math/mtx4.h>

#include <suprengine/tools/memory-profiler.h>

#include <unordered_map>

namespace suprengine
{
	class Shader
	{
	public:
		/*
		 * Creates, compiles and links GLSL shaders into one program.
		 * The vertex and fragment shaders must succeed or assertion is thrown.
		 */
		Shader(
			const char* vertex_code,
			const char* fragment_code,
			const char* tess_control_code = nullptr,
			const char* tess_eval_code = nullptr,
			const char* geometry_code = nullptr
		);
		~Shader();

		/*
		 * Sets program to be used for next rendering operations.
		 */
		void activate();

		/*
		 * Attempts to prepare the shader for the given frame tick.
		 * Returns whenever the shader has already been prepared for this frame.
		 * 
		 * This avoids sending uniforms that have already been sent earlier
		 * during the rendering.
		 */
		bool prepare( uint32 frame_tick );

		/*
		 * Returns whenever the shader is in a usable state.
		 */
		bool is_valid() const;

		void set_float( const char* name, float value );
		void set_int( const char* name, int value );
		void set_vec2( const char* name, float x, float y );
		void set_vec2( const char* name, const Vec2& value );
		void set_vec3( const char* name, float x, float y, float z );
		void set_vec3( const char* name, const Vec3& value );
		void set_vec4( const char* name, float x, float y, float z, float w );
		void set_color( const char* name, const Color& value );
		void set_mtx4( const char* name, const Mtx4& matrix );

		void print_all_params() const;

	public:
		static void* operator new( std::size_t bytes )
		{
			return MemoryProfiler::allocate( "Shader", bytes );
		}

	private:
		uint32 _create_shader( uint32 type, const char* code, const char* name );
		bool _compile_shader( uint32 shader_id, const char* name );

		void _create_program(
			uint32 vertex_shader_id,
			uint32 fragment_shader_id,
			uint32 tess_control_shader_id,
			uint32 tess_evaluation_shader_id,
			uint32 geometry_shader_id
		);
		bool _link_and_validate_program();

		void _retrieve_uniform_locations();
		int _get_uniform_location( const char* name ) const;

	private:
		uint32 _program_id = 0;
		uint32 _last_preparation_tick = 0;

		std::unordered_map<std::string, int> _uniform_locations {};
	};
}