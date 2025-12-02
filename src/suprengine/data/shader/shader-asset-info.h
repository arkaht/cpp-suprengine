#pragma once

#include <string>
#include <vector>

#include <suprengine/data/shader/shader-type.h>

namespace suprengine
{
	struct ShaderAssetInfo
	{
	public:
		std::string code_path {};
		ShaderType type {};
	};

	struct ShaderProgramAssetInfo
	{
	public:
		std::string name {};
		std::vector<ShaderAssetInfo> shaders {};
	};
}
