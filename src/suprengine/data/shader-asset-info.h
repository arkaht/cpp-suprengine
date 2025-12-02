#pragma once

#include <string>

namespace suprengine
{
	struct ShaderAssetInfo
	{
		std::string vertex_path {};
		std::string fragment_path {};

		std::string tesselation_control_path {};
		std::string tesselation_evaluation_path {};

		std::string geometry_path {};
	};
}
