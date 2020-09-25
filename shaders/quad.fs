#version 330 core

in vec2 tex_coord;

uniform vec4 u_colour;

out vec4 colour;


void main() {
	colour = u_colour;
}
