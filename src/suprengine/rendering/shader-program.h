#pragma once

#include <vector>
#include <unordered_map>

#include <suprengine/utils/usings.h>

namespace suprengine
{
	struct Vec2;
	struct Vec3;
	struct Color;
	struct Mtx4;

	class Shader;
}

namespace suprengine
{
	class ShaderProgram
    {
    public:
		/**
		 * Creates a shader program by linking and validating with the given shaders.
		 * @param name Program name, for debugging.
		 * @param shaders List of shaders to link with.
		 */
		explicit ShaderProgram( const std::string& name, const std::vector<Shader>& shaders );

		// Not copyable
    	ShaderProgram( const ShaderProgram& shader ) = delete;
    	ShaderProgram& operator=( const ShaderProgram& shader ) = delete;

		// Moveable
		ShaderProgram( ShaderProgram&& shader ) noexcept = default;
		ShaderProgram& operator=( ShaderProgram&& shader ) noexcept = default;

    	~ShaderProgram();

    public:
    	/**
		 * Sets program to be used for future rendering operations.
		 */
    	void activate();

    	/**
		 * Attempts to prepare the program for the given frame tick.
		 *
		 * This avoids sending uniforms that have already been sent earlier
		 * during the rendering (e.g. view projection matrix, ambient lighting).
		 *
		 * @return Whether the program must be prepared during this frame.
		 */
    	bool prepare( uint32 frame_tick );

    	void set_float( const char* name, float value );
    	void set_int( const char* name, int value );

    	void set_vec2( const char* name, const Vec2& value );
    	void set_vec3( const char* name, const Vec3& value );
    	void set_vec4( const char* name, float x, float y, float z, float w );

    	void set_color( const char* name, const Color& value );
    	void set_mtx4( const char* name, const Mtx4& matrix );

		void print_all_params() const;

		uint32 get_uniform_location( const char* name ) const;

		/**
		 * Returns whether the program is ready-for-use (i.e. linked and validated).
		 * @return Whether the program is valid.
		 */
		bool is_valid() const;

    	uint32 get_id() const;
		const std::string& get_name() const;

	public:
	#ifdef ENABLE_MEMORY_PROFILER
		void* operator new( std::size_t bytes );
	#endif

	private:
		bool link();
		bool validate();

		void update_uniform_locations();

	private:
		using UniformsLocationsMap = std::unordered_map<std::string, int>;

	private:
		std::string _name {};

		uint32 _id = 0;
		uint32 _last_preparation_tick = 0;

		UniformsLocationsMap _uniform_locations {};
    };
}