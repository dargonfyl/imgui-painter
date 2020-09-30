#version 330 core

layout (location = 0) in vec2 in_position;
layout (location = 1) in vec2 in_tex_coord;

uniform mat4 u_model;
uniform mat4 u_projection;

out vec2 tex_coord;


/*
	Do the following:
		1. Transform positions, then set gl_Position
		2, Preserve texture coordinates.
*/
void main() {
	tex_coord = in_tex_coord;

	gl_Position = u_projection * u_model * vec4(in_position, 0.0, 1.0);
	// gl_Position = vec4(in_position, 0.0, 1.0);
}
