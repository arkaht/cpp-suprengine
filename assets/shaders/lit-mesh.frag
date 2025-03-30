#version 330

uniform sampler2D u_texture;

uniform vec4 u_modulate;
uniform vec2 u_tiling;

uniform vec4 u_ambient_color;
uniform vec3 u_ambient_direction;
uniform float u_ambient_scale;

in vec2 uv;
in vec3 normal;
out vec4 out_color;

void main() 
{
	out_color = texture( u_texture, uv * u_tiling ) * u_modulate;

	//  Lighting
	float diffuse = -min( 
		dot( normal, u_ambient_direction ),
		0.0f
	);
	out_color = mix( u_ambient_color * u_ambient_scale, out_color, diffuse );
}