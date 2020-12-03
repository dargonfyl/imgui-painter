#version 330 core

in vec2 tex_coord;

uniform sampler2D u_sprite;
uniform bool u_premultiply;

out vec4 colour;

// NOTE: blending functions handled by GL. We need to premultiply the colours, though.
void main() {
	colour = texture(u_sprite, tex_coord);

	// TODO: WHAT THE HELL DO WE DO ABOUT THE PREMULTIPLYING THING
	colour.xyz *= colour.w;
	// if (u_premultiply) {
	// 	colour.xyz *= colour.w;  // Premultiply
	// } else {
	// 	colour.xyz /= colour.w;  // Undo premultiply
	// }
}
