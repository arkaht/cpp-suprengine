#version 330

uniform mat4 u_view_projection;

uniform vec3 u_origin;
uniform vec3 u_offset;

layout( location = 0 ) in vec3 in_position;

void main()
{
	vec4 pos = vec4( in_position, 1.0f );
	pos.xyz *= u_offset;
	pos.xyz += u_origin;

	gl_Position = pos * u_view_projection;
}