#version 330

uniform mat4 world_transform, view_projection;

in vec3 in_position;

void main()
{
	vec4 pos = vec4( in_position, 1.0f );
	gl_Position = pos * world_transform * view_projection;
}