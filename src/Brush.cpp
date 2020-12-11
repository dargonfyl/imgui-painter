#include "Brush.hpp"

#include <assert.h>
// #include <algorithm>  // UHHHHH


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
		R = static_cast<unsigned char>(color.x * 255.0f);
		G = static_cast<unsigned char>(color.y * 255.0f);
		B = static_cast<unsigned char>(color.z * 255.0f);
		A = static_cast<unsigned char>(color.w * 255.0f);
	}


	void Brush::set_color(unsigned char R, unsigned char G, unsigned char B, unsigned char A) {
		this->R = R;
		this->G = G;
		this->B = B;
		this->A = A;
	}


	void Brush::use(std::vector<unsigned char> &buffer, int x, int y, int width, int height) {
		assert(buffer.size() == 4 * width * height);

		// Square brush for now
		for (int i = std::max(0, x - size); i < std::min(width, x + size); i++) {
			for (int j = std::max(0, y - size); j < std::min(height, y + size); j++) {
				int pos = 4 * (j * width + i);
				buffer[pos] = R;
				buffer[pos + 1] = G;
				buffer[pos + 2] = B;
				buffer[pos + 3] = A;
			}
		}
	}
}  // namespace Im_Painter
