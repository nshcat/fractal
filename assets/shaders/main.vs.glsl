#version 450

// Vertex coordinates
const vec2 vertex_coords[] = vec2[6](
	vec2(1, 1),	// BR
	vec2(1, 0),	// TR
	vec2(0, 0),	// TL
	
	vec2(1, 1),	// BR
	vec2(0, 0),	// TL
	vec2(0, 1)	// BL
);

// Uniforms
uniform uvec2 window_size;
uniform mat4 projection_mat;

// Texture coordinates output
smooth out vec2 tex_coords;


void main()
{
	const vec2 t_pos = vertex_coords[gl_VertexID] * vec2(float(window_size.x), float(window_size.y));

	gl_Position = projection_mat * vec4(t_pos, 0.f, 1.f);
	
	tex_coords = vertex_coords[gl_VertexID];
}
