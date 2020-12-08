#include "Layer.hpp"

#include <string.h>  // memset
#include <stdlib.h>


namespace Im_Painter {
	Layer::Layer(layer_size_t height, layer_size_t width) {
		unsigned int total_bytes = 4 * height * width;
		unsigned char buf[total_bytes];

		memset(buf, static_cast<unsigned char>(50), total_bytes);

		texture = new Texture(width, height, 1, buf, RGBA, RGBA);
		visible = true;
	}


	Layer::Layer(unsigned char *data, layer_size_t height, layer_size_t width) {
		unsigned int total_bytes = 4 * height * width;
		texture = new Texture(width, height, 1, data, RGBA, RGBA);
		visible = true;
	}


	Layer::~Layer() {
		delete texture;
	}


	void Layer::merge(Layer &to_merge) {

	}


	void Layer::update(unsigned char *data) {
		texture->update(1, data);
	}


	Texture_id_t Layer::get_texture_id() {
		return texture->get_id();
	}


	void Layer::bind() {
		texture->bind();
	}
} // namespace Im_Painter
