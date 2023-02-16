#version 330

uniform mat4 u_world_transform, u_view_projection;
uniform vec2 u_origin;

layout( location = 0 ) in vec3 in_position;
layout( location = 1 ) in vec3 in_normal;
layout( location = 2 ) in vec2 in_uv;

out vec2 uv;

void main()
{
	vec4 pos = vec4( in_position, 1.0f );
	pos.x -= u_origin.x, pos.y -= u_origin.y;
	gl_Position = pos * u_world_transform * u_view_projection;

	uv = in_uv;
}