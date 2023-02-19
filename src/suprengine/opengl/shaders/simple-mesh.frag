#version 330

uniform vec4 u_modulate;
uniform sampler2D u_texture;
uniform vec4 u_source_rect;

in vec2 uv;
out vec4 out_color;

void main() 
{
	out_color = texture( u_texture, uv ) * u_modulate;
}