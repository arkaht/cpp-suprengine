#version 330

uniform vec4 u_modulate;
uniform sampler2D u_texture;
uniform vec4 u_source_rect;

in vec2 uv;
out vec4 out_color;

void main() 
{
	vec2 suv = vec2( 
		u_source_rect.x + uv.x * u_source_rect.z,
		u_source_rect.y + uv.y * u_source_rect.w
	);

	out_color = texture( u_texture, suv ) * u_modulate;
	if ( out_color.a < 0.1f ) discard;
}