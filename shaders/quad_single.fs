#version 330 core

in vec2 tex_coord;

uniform sampler2D u_sprite;
uniform bool u_premultiply;

uniform float u_hue_shift;
uniform float u_value_shift;
uniform float u_saturation_shift;

out vec4 colour;


vec3 rgb_to_hsv(float r, float g, float b) {
	float max_val = max(r, max(g, b));
	float min_val = min(r, min(g, b));

	// Hue
	float h; // in degrees
	if (max_val == min_val) {  // Simple default case. Do NOT increase saturation if this is the case!
		h = 0.0;
	} else if (max_val == r) {
		h = 60.0 * (0.0 + (g - b) / (max_val - min_val));
	} else if (max_val == g) {
		h = 60.0 * (2.0 + (b - r)/ (max_val - min_val));
	} else if (max_val == b) {
		h = 60.0 * (4.0 + (r - g) / (max_val - min_val));
	}

	if (h < 0.0) {
		h += 360.0;
	}

	float s = max_val == 0.0 ? 0.0 : (max_val - min_val) / max_val;

	float v = max_val;

	return vec3(h, s, v);
}



vec3 hsv_to_rgb(float h, float s, float v) {
	float r, g, b;

	float c = v * s;
	float h_ = mod(h / 60.0, 6); // For convenience, change to multiples of 60
	float x = c * (1.0 - abs(mod(h_, 2) - 1));

	float r_, g_, b_;
	if (0.0 <= h_ && h_ < 1.0) {
		r_ = c, g_ = x, b_ = 0.0;
	} else if (1.0 <= h_ && h_ < 2.0) {
		r_ = x, g_ = c, b_ = 0.0;
	} else if (2.0 <= h_ && h_ < 3.0) {
		r_ = 0.0, g_ = c, b_ = x;
	} else if (3.0 <= h_ && h_ < 4.0) {
		r_ = 0.0, g_ = x, b_ = c;
	} else if (4.0 <= h_ && h_ < 5.0) {
		r_ = x, g_ = 0.0, b_ = c;
	} else if (5.0 <= h_ && h_ < 6.0) {
		r_ = c, g_ = 0.0, b_ = x;
	} else {
		r_ = 0.0, g_ = 0.0, b_ = 0.0;
	}

	float m = v - c;
	r = r_ + m;
	g = g_ + m;
	b = b_ + m;
	return vec3(r, g, b);
}



// NOTE: blending functions handled by GL. We need to premultiply the colours, though.
void main() {
	colour = texture(u_sprite, tex_coord);

	if (u_hue_shift != 0.0 || u_value_shift != 0.0 || u_saturation_shift != 0.0) {
		vec3 hsv = rgb_to_hsv(colour.x, colour.y, colour.z);

		// shift
		hsv.x += u_hue_shift;
		if (colour.x != colour.y || colour.y != colour.z) {  // If not grayscale
			hsv.y = u_saturation_shift <= 0.0 ? hsv.y * (1.0 + u_saturation_shift) : hsv.y + (1.0 - hsv.y) * u_saturation_shift;
		}
		hsv.z = u_value_shift <= 0.0 ? hsv.z * (1.0 + u_value_shift) : hsv.z + (1.0 - hsv.z) * u_value_shift;

		vec3 rgb = hsv_to_rgb(hsv.x, hsv.y, hsv.z);

		colour.xyz = rgb;
	}


	// This is here if you want to premultiply for the correct final results.
	if (u_premultiply)
		colour.xyz *= colour.w;
}
