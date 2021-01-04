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


	void Brush::set_size(Brush_size_t size) {
		this->size = size;
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


	glm::vec4 Brush::get_color() {
		float r = static_cast<float>(R) / 255.0f;
		float g = static_cast<float>(G) / 255.0f;
		float b = static_cast<float>(B) / 255.0f;
		float a = static_cast<float>(A) / 255.0f;

		return glm::vec4(r, g, b, a);
	}


	void Brush::use(std::vector<unsigned char> &buffer, int x, int y, int width, int height) {
		assert(buffer.size() == 4 * width * height);

		unsigned char cols[] = {
			R,
			G,
			B
		};

		// Square brush for now
		for (int i = std::max(0, x - size); i < std::min(width, x + size); i++) {
			for (int j = std::max(0, y - size); j < std::min(height, y + size); j++) {
				int pos = 4 * (j * width + i);

				float A_alpha = A / 255.0f;
				float B_alpha = buffer[pos + 3] / 255.0f;
				float comp_alpha = A_alpha + B_alpha * (1.0f - A_alpha);
				for (int k = 0; k < 3; k++) {
					float col_a = cols[k] / 255.0f;
					float col_b = buffer[pos + k] / 255.0f;
					float col = (A_alpha * col_a + B_alpha * col_b * (1 - A_alpha)) / comp_alpha;
					buffer[pos + k] = static_cast<unsigned char>(col * 255.0f);
				}

				if (comp_alpha >= A_alpha) {
					comp_alpha *= 1.0 + A_alpha;
					comp_alpha = std::min(1.0f, comp_alpha);
				}

				buffer[pos + 3] = static_cast<unsigned char>(comp_alpha * 255.0f);
			}
		}
	}
}  // namespace Im_Painter
