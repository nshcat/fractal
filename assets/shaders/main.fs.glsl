#version 450

layout(binding=0) uniform sampler2D main_tex;

// Main fragment shader output
layout (location = 0) out vec4 fragmentColor;

smooth in vec2 tex_coords;

void main()
{
	fragmentColor = texture2D(main_tex, tex_coords, 0);
}
