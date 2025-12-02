#pragma once

#include <suprengine/utils/usings.h>

#include <suprengine/data/shader/shader-type.h>

namespace suprengine
{
	class Shader
	{
	public:
		/**
		 * Creates a shader by compiling its code.
		 * @param name Shader name, for debugging.
		 * @param code Source code, in GLSL.
		 * @param type Shader type to use for compiling the code.
		 */
		explicit Shader( const std::string& name, const std::string& code, ShaderType type );

		// Not copyable
		Shader( const Shader& shader ) = delete;
		Shader& operator=( const Shader& shader ) = delete;

		// Moveable
		Shader( Shader&& shader ) noexcept = default;
		Shader& operator=( Shader&& shader ) noexcept = default;

		~Shader();

	public:
		/**
		 * Returns whether the shader is ready-for-use (i.e. compiled).
		 * @return Whether the shader is valid.
		 */
		bool is_valid() const;

		uint32 get_id() const;
		const std::string& get_name() const;

	private:
		bool compile();

	private:
		std::string _name {};
		std::string _code {};

		uint32 _id = 0;
		ShaderType _type = ShaderType::Vertex;
	};
}
