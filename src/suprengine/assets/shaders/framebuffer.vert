#version 330

/* layout( location = 0 ) in vec3 in_position;
layout( location = 1 ) in vec3 in_norm;
layout( location = 2 ) in vec2 in_uv; */
layout( location = 0 ) in vec2 in_position;
layout( location = 1 ) in vec2 in_uv;

out vec2 uv;

void main()
{
    gl_Position = vec4( in_position.x, in_position.y, 0.0, 1.0 ); 
    
	uv = in_uv;
}  