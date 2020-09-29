#version 330 core

in vec2 tex_coord;

uniform vec4 u_colour;
uniform sampler2DArray u_sprite; // TODO: This will need to be an array texture at some point. 
// uniform sampler2D u_sprite;
out vec4 colour;


void main() {
	colour = u_colour * texture(u_sprite, vec3(tex_coord, 0.0));
	// colour = u_colour * texture(u_sprite, tex_coord);
}
