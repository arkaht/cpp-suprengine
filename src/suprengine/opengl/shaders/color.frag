#version 330

uniform vec4 modulate;

out vec4 out_color;

void main() 
{
	out_color = modulate;
}