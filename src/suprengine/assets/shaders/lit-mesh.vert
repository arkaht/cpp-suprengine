#version 330

uniform mat4 u_world_transform, u_view_projection;

layout( location = 0 ) in vec3 in_position;
layout( location = 1 ) in vec3 in_normal;
layout( location = 2 ) in vec2 in_uv;

out vec2 uv;
out vec3 normal;

void main() 
{
	vec4 pos = vec4( in_position, 1.0f );
	gl_Position = pos * u_world_transform * u_view_projection;

	uv = in_uv;
	normal = in_normal;
}