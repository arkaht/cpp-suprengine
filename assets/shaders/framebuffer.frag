#version 330

uniform sampler2D screen_texture;

in vec2 uv;
out vec4 out_color;

void main()
{
    out_color = texture( screen_texture, uv );
}