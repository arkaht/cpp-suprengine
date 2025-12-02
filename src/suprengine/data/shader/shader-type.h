#pragma once

#include <suprengine/utils/assert.h>
#include <suprengine/utils/usings.h>

namespace suprengine
{
	enum class ShaderType : uint8
	{
		Vertex,
		Fragment,
		TessellationControl,
		TessellationEvaluation,
		Geometry,
		Compute,
	};

	/**
	 * Returns a human-readable name of the shader type for logging purposes.
	 * @param type Shader type
	 * @return Name of the shader type
	 */
	inline const char* get_shader_type_name( const ShaderType type )
	{
		switch ( type )
		{
			case ShaderType::Vertex:
				return "vertex";
			case ShaderType::Fragment:
				return "fragment";
			case ShaderType::TessellationControl:
				return "tessellation control";
			case ShaderType::TessellationEvaluation:
				return "tessellation evaluation";
			case ShaderType::Geometry:
				return "geometry";
			case ShaderType::Compute:
				return "compute";
		}

		ASSERT( false );
		return "N/A";
	}
}