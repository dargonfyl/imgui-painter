#include "Canvas.hpp"

#include <string.h>  // be careful with this...
#include <assert.h>

#include <iostream>

namespace Im_Painter
{
	Canvas::Canvas(canvas_size_t height, canvas_size_t width) {
		assert(height > 0 && width > 0);
		this->height = height;
		this->width = width;

		layers.push_back(Layer(height, width));
	}


	Canvas::Canvas(unsigned char *data, canvas_size_t height, canvas_size_t width) {
		assert(height > 0 && width > 0);
		this->height = height;
		this->width = width;

		layers.push_back(Layer(data, height, width));
	}


	Texture Canvas::to_texture() {
		// Note: this is dangerous, but should work!
		// Instead of memcopy, use appends
		assert(layers.size() > 0);
		unsigned int layer_size = height * width * 4;
		unsigned char data[layer_size * layers.size()];
		for (int i = 0; i < layers.size(); i++) {
			std::vector<unsigned char> &layer_data = layers[i].get_data();
			memcpy(data + i * layer_size, &layer_data[0], layer_data.size());
		}

		return Texture(width, height, layers.size(), data, RGBA, RGBA);
	}


	canvas_num_layers_t Canvas::get_num_layers() {
		return this->layers.size();
	}


	void Canvas::new_layer() {

	}


	void Canvas::new_layer(unsigned char *data, canvas_size_t height, canvas_size_t width) {
		assert(height == this->height);  // These asserts can stay as asserts.
		assert(width == this->width);

		layers.push_back(Layer(data, height, width));
	}
} // namespace Im_Painter

