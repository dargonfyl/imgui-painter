#version 330 core

in vec2 tex_coord;

uniform vec4 u_colour;
uniform sampler2DArray u_sprite;
uniform int u_num_layers;

out vec4 colour;


vec4 alpha_composite(vec4 front, vec4 back) {
	float alpha_front = front.w;
	float alpha_back = back.w;

	vec3 col_front = front.xyz * alpha_front;
	vec3 col_back = back.xyz * alpha_back;

	vec3 col_final = col_front + col_back * (1.0 - alpha_front);
	float alpha_final = alpha_front + alpha_back * (1.0 - alpha_front);
	return vec4(col_final, alpha_final);
}


void main() {
	colour = u_colour * alpha_composite(texture(u_sprite, vec3(tex_coord, 1.0)), texture(u_sprite, vec3(tex_coord, 0.0)));

	// colour = u_colour * texture(u_sprite, tex_coord);
}
