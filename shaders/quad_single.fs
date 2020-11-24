#version 330 core

in vec2 tex_coord;

uniform sampler2D u_sprite;

out vec4 colour;

// NOTE: that blending must be handled by GL via the buffers
void main() {
	colour = texture(u_sprite, tex_coord);
}
