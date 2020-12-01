#include "Layer.hpp"

#include <string.h>  // memset
#include <stdlib.h>

namespace Im_Painter {
	Layer::Layer(layer_size_t height, layer_size_t width) {
		unsigned int total_bytes = 4 * height * width;
		unsigned char buf[total_bytes];
		memset(buf, static_cast<unsigned char>(50), total_bytes);
		// this->data = std::vector<unsigned char>(buf, buf + total_bytes);
		// texture = new Texture(width, height, 1, &this->data[0], RGBA, RGBA);
		texture = new Texture(width, height, 1, buf, RGBA, RGBA);
	}


	Layer::Layer(unsigned char *data, layer_size_t height, layer_size_t width) {
		unsigned int total_bytes = 4 * height * width;
		// this->data = std::vector<unsigned char>(data, data + total_bytes);  // Iterator version, start to end (inclusive)
		texture = new Texture(width, height, 1, data, RGBA, RGBA);
	}


	Layer::~Layer() {
		delete texture;
	}


	void Layer::merge(Layer &to_merge) {

	}

	std::vector<unsigned char> &Layer::get_data() {
		// return this->data;
	}


	void Layer::bind() {
		texture->bind();
	}
} // namespace Im_Painter
