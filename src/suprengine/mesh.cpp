#include "mesh.h"

#include <suprengine/assets.h>

using namespace suprengine;

Shader* Mesh::get_shader()
{
	return Assets::get_shader( shader_name );
}
