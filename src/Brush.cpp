#include "Brush.hpp"

#include <assert.h>


namespace Im_Painter {
	Brush::Brush() {
		size = 1;
		R = 0;
		G = 0;
		B = 0;
		A = 255;
	}


	Brush::Brush(Brush_size_t size) {
		this->size = size;
		R = 0;
		G = 0;
		B = 0;
		A = 255;
	}


	Brush::~Brush() {

	}


	void Brush::set_color(glm::vec4 color) {
		assert(0.0 <= color.x && color.x <= 1.0);
		assert(0.0 <= color.y && color.y <= 1.0);
		assert(0.0 <= color.z && color.z <= 1.0);
		assert(0.0 <= color.w && color.w <= 1.0);
		R = static_cast<unsigned char>(color.x * 255);
		G = static_cast<unsigned char>(color.y * 255);
		B = static_cast<unsigned char>(color.z * 255);
		A = static_cast<unsigned char>(color.w * 255);
	}


	void Brush::set_color(unsigned char R, unsigned char G, unsigned char B, unsigned char A) {
		this->R = R;
		this->G = G;
		this->B = B;
		this->A = A;
	}


	void Brush::use(unsigned char *pixel) {
		assert(pixel);
		*pixel = R;
		*(pixel + 1) = G;
		*(pixel + 2) = B;
		*(pixel + 3) = A;
	}
}  // namespace Im_Painter
