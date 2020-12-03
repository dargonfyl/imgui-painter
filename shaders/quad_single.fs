#version 330 core

in vec2 tex_coord;

uniform sampler2D u_sprite;

out vec4 colour;

// NOTE: blending functions handled by GL. We need to premultiply the colours, though.
void main() {
	colour = texture(u_sprite, tex_coord);
	colour.xyz *= colour.w;  // Premultiply
}
