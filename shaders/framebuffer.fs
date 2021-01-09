#version 330 core

in vec2 tex_coord;

uniform sampler2D u_sprite;

out vec4 colour;


void main() {
	colour = texture(u_sprite, vec2(tex_coord.x, -tex_coord.y));
	colour /= colour.w;
}
