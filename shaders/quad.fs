#version 330 core

in vec2 tex_coord;

uniform vec4 u_colour;
uniform sampler2D u_sprite; // TODO: This will need to be an array texture at some point. 

out vec4 colour;


void main() {
	colour = u_colour * texture(u_sprite, tex_coord);
}
